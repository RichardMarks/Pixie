//*** gif.cpp ***

/**
 * This is a heavily modified version of the Winimage code by Juan Soulie. 
 * The original copyright notice for winimage can be found below.
 *
 *												/Mattias Gustavsson
 */

//  --------------------------------------------------------------------------
//
// Copyright © 2000, Juan Soulie <jsoulie@cplusplus.com>
//
// Permission to use, copy, modify, distribute and sell this software or any
// part thereof and/or its documentation for any purpose is granted without fee
// provided that the above copyright notice and this permission notice appear
// in all copies.
//
// This software is provided "as is" without express or implied warranty of
// any kind. The author shall have no liability with respect to the
// infringement of copyrights or patents that any modification to the content
// of this file or this file itself may incur.
//
// ****************************************************************************

#include "gif.h"
#include "Asset.h"
#include "StandardLibrary.h"

GifLoader::~GifLoader()
	{
	for (int i=0; i<cels_.GetItemCount(); i++)
		{
		Cel* cel=cels_.Get(i);
		delete[] cel->pixels;
		delete cel;
		}

	}


int LZWDecoder (char * bufIn, char * bufOut,
				short InitCodeSize, int AlignedWidth,
				int Width, int Height, const int Interlace);


//*** Load ***

bool GifLoader::Load(const Asset& asset)
{

	struct GIFGCEtag {				// GRAPHIC CONTROL EXTENSION
		unsigned char BlockSize;		// Block Size: 4 bytes
		unsigned char PackedFields;		// Packed Fields. Bits detail:
										//    0: Transparent Color Flag
										//    1: User Input Flag
										//  2-4: Disposal Method
		unsigned short Delay;			// Delay Time (1/100 seconds)
		unsigned char Transparent;		// Transparent Color Index
	} gifgce;

	MemSet(&gifgce,0,sizeof(gifgce));

	int GraphicExtensionFound = 0;

	// OPEN FILE
	if (!asset.Open())
		{
		return false;
		}

	// *1* READ HEADER (SIGNATURE + VERSION)
	char szSignature[6];				// First 6 bytes (GIF87a or GIF89a)
	asset.Read(szSignature,6);
	if (StrNCmp(szSignature,"GIF",3)!=0)
		{
		asset.Close();
		return false;
		}

	// *2* READ LOGICAL SCREEN DESCRIPTOR
//	struct GIFLSDtag {
		unsigned char PackedFields;		// Packed Fields. Bits detail:
										//  0-2: Size of Global Color Table
										//    3: Sort Flag
										//  4-6: Color Resolution
										//    7: Global Color Table Flag
		unsigned char Background;		// Background Color Index
		unsigned char PixelAspectRatio;	// Pixel Aspect Ratio
//	} giflsd;

	asset.Read(&width_);
	asset.Read(&height_);
	asset.Read(&PackedFields);
	asset.Read(&Background);
	asset.Read(&PixelAspectRatio);

	// Global GIF variables:
	int GlobalBPP;						// Bits per Pixel.
	unsigned int* GlobalColorMap;		// Global colormap (allocate)

	GlobalBPP = (PackedFields & 0x07) + 1;

	// fill some animation data:
	loopCount_ = 0;

	// *3* READ/GENERATE GLOBAL COLOR MAP
	GlobalColorMap = new unsigned int [1<<GlobalBPP];
	if (PackedFields & 0x80)	// File has global color map?
		for (int n=0;n< 1<<GlobalBPP;n++)
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			asset.Read(&r);
			asset.Read(&g);
			asset.Read(&b);
			GlobalColorMap[n]=0xff000000;
			GlobalColorMap[n]|=r<<16;
			GlobalColorMap[n]|=g<<8;
			GlobalColorMap[n]|=b;
		}

	else	// GIF standard says to provide an internal default Palette:
		for (int n=0;n<256;n++)
			GlobalColorMap[n]=0xff000000 | n<<16 | n << 8 | n;

	// *4* NOW WE HAVE 3 POSSIBILITIES:
	//  4a) Get and Extension Block (Blocks with additional information)
	//  4b) Get an Image Separator (Introductor to an image)
	//  4c) Get the trailer Char (End of GIF File)
	do
	{
		unsigned char charGot;
		asset.Read(&charGot);

		if (charGot == 0x21)		// *A* EXTENSION BLOCK 
		{
			unsigned char value;
			asset.Read(&value);
			switch (value)
			{

			case 0xF9:			// Graphic Control Extension
				asset.Read(&gifgce.BlockSize);
				asset.Read(&gifgce.PackedFields);
				asset.Read(&gifgce.Delay);
				asset.Read(&gifgce.Transparent);
				GraphicExtensionFound++;
				unsigned char terminator;
				asset.Read(&terminator); // Block Terminator (always 0)
				break;

			case 0xFE:			// Comment Extension: Ignored
			case 0x01:			// PlainText Extension: Ignored
			case 0xFF:			// Application Extension: Ignored
			default:			// Unknown Extension: Ignored
				// read (and ignore) data sub-blocks
				unsigned char nBlockLength;
				asset.Read(&nBlockLength);
				while (nBlockLength>0)
					{
					asset.Seek(nBlockLength,Asset::SEEK_FROM_CURRENT);
					asset.Read(&nBlockLength);
					}
				break;
			}
		}


		else if (charGot == 0x2c) {	// *B* IMAGE (0x2c Image Separator)

			unsigned short xPos;			// Image Left Position
			unsigned short yPos;			// Image Top Position
			unsigned short Width;			// Image Width
			unsigned short Height;			// Image Height
			unsigned char PackedFields;		// Packed Fields. Bits detail:
										//  0-2: Size of Local Color Table
										//  3-4: (Reserved)
										//    5: Sort Flag
										//    6: Interlace Flag
										//    7: Local Color Table Flag
			asset.Read(&xPos);
			asset.Read(&yPos);
			asset.Read(&Width);
			asset.Read(&Height);
			asset.Read(&PackedFields);

			int LocalColorMap = (PackedFields & 0x08)? 1 : 0;

			Cel* cel=new Cel;
			cels_.Add(cel);

			cel->x=xPos;
			cel->y=yPos;
			cel->width=Width;
			cel->height=Height;


			if (GraphicExtensionFound)
			{
				cel->delay=gifgce.Delay;
				if (gifgce.PackedFields&0x01)
					{
					cel->transparency=gifgce.Transparent;
					}
				else
					{
					cel->transparency=-1;
					}
			}
		
			if (LocalColorMap)		// Read Color Map (if descriptor says so)
				{
				int BPP=LocalColorMap ? (PackedFields&7)+1 : GlobalBPP;
				for (int i=0; i<1<<BPP; i++)
					{
					unsigned char r;
					unsigned char g;
					unsigned char b;
					asset.Read(&r);
					asset.Read(&g);
					asset.Read(&b);
					unsigned int c=0xff000000;
					c=0xff000000;
					c|=r<<16;
					c|=g<<8;
					c|=b;
					cel->palette[i]=c;
					}
				}

			else
				{
				// Otherwise copy Global
				int BPP=LocalColorMap ? (PackedFields&7)+1 : GlobalBPP;
				for (int i=0; i<1<<BPP; i++)
					{
					cel->palette[i]=GlobalColorMap[i];
					}
				}

			unsigned char firstbyte;
			asset.Read(&firstbyte);

			// Calculate compressed image block size
			// to fix: this allocates an extra byte per block
			long ImgStart,ImgEnd;				
			ImgEnd = ImgStart = asset.Tell();
			unsigned char n;
			asset.Read(&n);
			while (n)
				{
				asset.Seek(ImgEnd+=n+1);
				asset.Read(&n);
				}

			asset.Seek(ImgStart);

			// Allocate Space for Compressed Image
			char * pCompressedImage = new char [ImgEnd-ImgStart+4];
  
			// Read and store Compressed Image
			char * pTemp = pCompressedImage;
			unsigned char nBlockLength;
			asset.Read(&nBlockLength);
			while (nBlockLength)
			{
				asset.Read(pTemp,nBlockLength);
				pTemp+=nBlockLength;
				asset.Read(&nBlockLength);
			}

			cel->pixels=new unsigned char[Width*Height];
			
			// Call LZW/GIF decompressor
			int l=LZWDecoder(
				(char*) pCompressedImage,
				(char*) cel->pixels,
				firstbyte, Width,
				Width, Height,
				((PackedFields & 0x40)?1:0)	//Interlaced?
				);

			delete[]pCompressedImage;
			GraphicExtensionFound=0;
		}


		else if (charGot == 0x3b) {	// *C* TRAILER: End of GIF Info
			break; // Ok. Standard End.
		}

	} while (asset.Tell()<asset.GetSize());

	delete[] GlobalColorMap;
/*	giffile.close();
	if (nImages==0) ERRORMSG("Premature End Of File");
	return nImages;
*/
	return true;
}

// ****************************************************************************
// * LZWDecoder (C/C++)                                                       *
// * Codec to perform LZW (GIF Variant) decompression.                        *
// *                         (c) Nov2000, Juan Soulie <jsoulie@cplusplus.com> *
// ****************************************************************************
//
// Parameter description:
//  - bufIn: Input buffer containing a "de-blocked" GIF/LZW compressed image.
//  - bufOut: Output buffer where result will be stored.
//  - InitCodeSize: Initial CodeSize to be Used
//    (GIF files include this as the first byte in a picture block)
//  - AlignedWidth : Width of a row in memory (including alignment if needed)
//  - Width, Height: Physical dimensions of image.
//  - Interlace: 1 for Interlaced GIFs.
//
int LZWDecoder (char * bufIn, char * bufOut,
				short InitCodeSize, int AlignedWidth,
				int Width, int Height, const int Interlace)
{
	const int MaxTableSize = 65535;
	int n;
	int row=0,col=0;				// used to point output if Interlaced
	int nPixels, maxPixels;			// Output pixel counter

	short CodeSize;					// Current CodeSize (size in bits of codes)
	short ClearCode;				// Clear code : resets decompressor
	short EndCode;					// End code : marks end of information

	long whichBit;					// Index of next bit in bufIn
	long LongCode;					// Temp. var. from which Code is retrieved
	short Code;						// Code extracted
	short PrevCode=0;					// Previous Code
	short OutCode;					// Code to output

	// Translation Table:
	short Prefix[MaxTableSize];				// Prefix: index of another Code
	unsigned char Suffix[MaxTableSize];		// Suffix: terminating character
	short FirstEntry;				// Index of first free entry in table
	short NextEntry;				// Index of next free entry in table

	unsigned char OutStack[MaxTableSize];	// Output buffer
	int OutIndex;					// Characters in OutStack

	int RowOffset;					// Offset in output buffer for current row

	// Set up values that depend on InitCodeSize Parameter.
	CodeSize = InitCodeSize+1;
	ClearCode = (1 << InitCodeSize);
	EndCode = ClearCode + 1;
	NextEntry = FirstEntry = ClearCode + 2;

	whichBit=0;
	nPixels = 0;
	maxPixels = Width*Height;
	RowOffset =0;

	while (nPixels<maxPixels) {
		OutIndex = 0;							// Reset Output Stack

		// GET NEXT CODE FROM bufIn:
		// LZW compression uses code items longer than a single byte.
		// For GIF Files, code sizes are variable between 9 and 12 bits 
		// That's why we must read data (Code) this way:
		LongCode=*((long*)(bufIn+whichBit/8));	// Get some bytes from bufIn
		LongCode>>=(whichBit&7);				// Discard too low bits
		Code =(short)(LongCode & ((1<<CodeSize)-1) );	// Discard too high bits
		whichBit += CodeSize;					// Increase Bit Offset

		// SWITCH, DIFFERENT POSIBILITIES FOR CODE:
		if (Code == EndCode)					// END CODE
			break;								// Exit LZW Decompression loop

		if (Code == ClearCode) {				// CLEAR CODE:
			CodeSize = InitCodeSize+1;			// Reset CodeSize
			NextEntry = FirstEntry;				// Reset Translation Table
			PrevCode=Code;				// Prevent next to be added to table.
			continue;							// restart, to get another code
		}
		if (Code < NextEntry)					// CODE IS IN TABLE
			OutCode = Code;						// Set code to output.

		else {									// CODE IS NOT IN TABLE:
			OutIndex++;			// Keep "first" character of previous output.
			OutCode = PrevCode;					// Set PrevCode to be output
		}

		// EXPAND OutCode IN OutStack
		// - Elements up to FirstEntry are Raw-Codes and are not expanded
		// - Table Prefices contain indexes to other codes
		// - Table Suffices contain the raw codes to be output
		while (OutCode >= FirstEntry) {
			if (OutIndex > MaxTableSize) 
				{
				return 0;
				}
			OutStack[OutIndex++] = Suffix[OutCode];	// Add suffix to Output Stack
			OutCode = Prefix[OutCode];				// Loop with preffix
		}

		// NOW OutCode IS A RAW CODE, ADD IT TO OUTPUT STACK.
		if (OutIndex > MaxTableSize) 
			{
			return 0;
			}
		OutStack[OutIndex++] = (unsigned char) OutCode;

		// ADD NEW ENTRY TO TABLE (PrevCode + OutCode)
		// (EXCEPT IF PREVIOUS CODE WAS A CLEARCODE)
		if (PrevCode!=ClearCode) {
			Prefix[NextEntry] = PrevCode;
			Suffix[NextEntry] = (unsigned char) OutCode;
			NextEntry++;

			// Prevent Translation table overflow:
			if (NextEntry>=MaxTableSize)
				{
				return 0;
				}
      
			// INCREASE CodeSize IF NextEntry IS INVALID WITH CURRENT CodeSize
			if (NextEntry >= (1<<CodeSize)) {
				if (CodeSize < 12) CodeSize++;
				else {}				// Do nothing. Maybe next is Clear Code.
			}
		}

		PrevCode = Code;

		// Avoid the possibility of overflow on 'bufOut'.
		if (nPixels + OutIndex > maxPixels) OutIndex = maxPixels-nPixels;

		// OUTPUT OutStack (LAST-IN FIRST-OUT ORDER)
		for (n=OutIndex-1; n>=0; n--) {
			if (col==Width)						// Check if new row.
			{
				if (Interlace) {				// If interlaced::
					     if ((row&7)==0) {row+=8; if (row>=Height) row=4;}
					else if ((row&3)==0) {row+=8; if (row>=Height) row=2;}
					else if ((row&1)==0) {row+=4; if (row>=Height) row=1;}
					else row+=2;
				}
				else							// If not interlaced:
					row++;

				RowOffset=row*AlignedWidth;		// Set new row offset
				col=0;
			}
			bufOut[RowOffset+col]=OutStack[n];	// Write output
			col++;	nPixels++;					// Increase counters.
		}

	}	// while (main decompressor loop)

	return whichBit;
}
