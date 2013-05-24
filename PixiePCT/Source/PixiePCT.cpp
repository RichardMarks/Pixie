//*** PixiePCT.cpp ***

#include "PixiePCT.h"

#include <stdio.h>
#include "CommandLine.h"
#include "Image.h"
#include "Asset.h"
#include "StandardLibrary.h"
#include "Bitmap_16bit.h"
#include "Bitmap_16bitAlpha.h"
#include "Bitmap_16bitAlphaCrop.h"
#include "Bitmap_Alpha.h"
#include "Bitmap_RLE8.h"
#include "Bitmap_RLE16.h"
#include "ImageFormat_TGA.h"
#include "ImageFormat_GIF.h"
#include "ImageFormat_PNG.h"
#include "ImageFormat_JPG.h"
#include "Platform_FileSystem.h"
#include "Platform_FileSystem_Directory.h"
#include "Platform_FileSystem_File.h"

bool format16bit=false;
bool format16bitAlpha=false;
bool format16bitAlphaCrop=false;
bool formatAlpha=false;
bool formatAlphaGS=false;
bool formatRLE8=false;
bool formatRLE16=false;
bool formatTGA=false;
bool formatPNG=false;


//*** PixiePCTMain ***

void PixiePCTMain(const char* commandLine)
	{
	ImageFormat_PNG::Register();
	ImageFormat_JPG::Register();
	ImageFormat_GIF::Register();
	ImageFormat_TGA::Register();

	CommandLine cmd(commandLine);

	printf("Pixie Picture Conversion Tool 1.0\n\n");

	// Get the flags
	bool strip=cmd.IsCommandLineFlagSet("strip");
	bool dither=cmd.IsCommandLineFlagSet("dither");

	printf("Dither: %s\nStrip: %s\n\n",dither?"YES":"NO",strip?"YES":"NO");

	format16bit=cmd.IsCommandLineFlagSet("16bit");
	format16bitAlpha=cmd.IsCommandLineFlagSet("16bitalpha");
	format16bitAlphaCrop=cmd.IsCommandLineFlagSet("16bitalphacrop");
	formatAlpha=cmd.IsCommandLineFlagSet("alpha");
	formatAlphaGS=cmd.IsCommandLineFlagSet("alphags");
	formatRLE8=cmd.IsCommandLineFlagSet("rle8");
	formatRLE16=cmd.IsCommandLineFlagSet("rle16");
	formatTGA=cmd.IsCommandLineFlagSet("tga");
	formatPNG=cmd.IsCommandLineFlagSet("png");

	// Make sure exactly one format have been requested
	int formatCount=0;
	if (format16bit) formatCount++;
	if (format16bitAlpha) formatCount++;
	if (format16bitAlphaCrop) formatCount++;
	if (formatAlpha) formatCount++;
	if (formatAlphaGS) formatCount++;
	if (formatRLE8) formatCount++;
	if (formatRLE16) formatCount++;
	if (formatCount==0)
		{
		printf("ERROR: No conversion requested.\n");
		return;
		}
	if (formatCount>1)
		{
		printf("ERROR: Requested conversion to multiple formats.\n");
		return;
		}

	// Load all images/fonts
	Array<ImageFile> images;
	Array<FontFile> fonts;
	printf("Loading images/fonts:\n");
	for (int i=1; i<cmd.GetCommandLineStringCount(); i++)
		{
		const char* filename=cmd.GetCommandLineString(i).GetString();

		// Is this a file?
		Platform_FileSystem_File* file=Platform::GetPlatform_FileSystem()->CreateFileObject(filename);
		if (file->Exists())
			{
			LoadFile(filename,images,fonts,!strip,dither);
			}
		delete file;

		// Is this a directory?
		Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(filename);
		if (directory->Exists())
			{
			// Recursively add all images found in this directory
			LoadDirectory(filename,images,fonts,!strip,dither);
			}
		delete directory;
		}

	if (!strip)
		{
		return;
		}

	// Make sure we have images to convert
	if (images.GetItemCount()<=0 && fonts.GetItemCount()<=0)
		{
		printf("ERROR: No image/font file(s) specified.\n");
		return;
		}


	// Perform main operations (all specified)
	if (format16bit)
		{
		printf("\nConverting to 16bit:");
		Convert<Bitmap_16bit>(images,strip,dither,"PIX16BIT");
		Convert<Bitmap_16bit>(fonts,dither,"FNT16BIT");
		}
	if (format16bitAlpha)
		{
		printf("\nConverting to 16bitAlpha:");
		Convert<Bitmap_16bitAlpha>(images,strip,dither,"PIX16BAL");
		Convert<Bitmap_16bitAlpha>(fonts,dither,"FNT16BAL");
		}
	if (format16bitAlphaCrop)
		{
		printf("\nConverting to 16bitAlphaCrop:");
		Convert<Bitmap_16bitAlphaCrop>(images,strip,dither,"PIX16BAC");
		Convert<Bitmap_16bitAlphaCrop>(fonts,dither,"FNT16BAC");
		}
	if (formatAlpha)
		{
		printf("\nConverting to Alpha:");
		Convert<Bitmap_Alpha>(images,strip,false,"PIXALPHA");
		Convert<Bitmap_Alpha>(fonts,false,"FNTALPHA");
		}
	if (formatAlphaGS)
		{
		printf("\nConverting to AlphaGS:");
		Convert<Bitmap_Alpha>(images,strip,true,"PIXALPHA");
		Convert<Bitmap_Alpha>(fonts,true,"FNTALPHA");
		}
	if (formatRLE8)
		{
		printf("\nConverting to RLE8:");
		Convert<Bitmap_RLE8>(images,strip,dither,"PIXRLE8B");
		Convert<Bitmap_RLE8>(fonts,dither,"FNTRLE8B");
		}
	if (formatRLE16)
		{
		printf("\nConverting to RLE16:");
		Convert<Bitmap_RLE16>(images,strip,dither,"PIXRLE16");
		Convert<Bitmap_RLE16>(fonts,dither,"FNTRLE16");
		}
	}




//*** GetPixieFilename ***

StringId GetPixieFilename(StringId filename, bool stripNumbers, const char* extension)
	{
	const char* in=filename.GetString();
	char* out=new char[StrLen(in)+2+StrLen(extension)];
	StrCpy(out,in);
	const char* lastDelimiter=StrRChr(out,'/');
	const char* lastDelimiter2=StrRChr(out,'\\');
	const char* extensionStart=StrRChr(out,'.');
	
	// If we have no extension (no dot, or the dot is in the path, not in the name)
	if (extensionStart==0 || (lastDelimiter>extensionStart || lastDelimiter2>extensionStart))
		{
		extensionStart=out+StrLen(out);
		}
	else
		{
		extensionStart--;
		}
	const char* end=extensionStart;
	
	// Strip numbers
	if (stripNumbers)
		{
		while(*end>='0' && *end<='9' && *end!='/' && *end!='\\')
			{
			end--;
			}
		if (*end=='_')
			{
			end--;
			}
		}

	// Add the extension
	StrCpy((char*)end+1,extension);


	StringId result(out);
	delete[] out;
	return result;
	}


//*** Convert ***

template <class Type> 
void Convert(Array<ImageFile>& images,bool strip, bool dither, const char* header)
	{ 
	if (images.GetItemCount()==0)
		{
		return;
		}

	Array<Type*> result;
	for (int i=0; i<images.GetItemCount(); i++)
		{
		printf("\n\t%s",images.Get(i).filename.GetString());
		result.Add(new Type(images.Get(i).image,dither));
		}

	printf("\nSaving images");
	if (!strip)
		{
		for (int i=0; i<result.GetItemCount(); i++)
			{
			StringId newFilename=GetPixieFilename(images.Get(i).filename);
			printf("\n\t%s",newFilename.GetString());
			Asset asset(newFilename.GetString());
			result.Get(i)->Save(asset);
			delete result.Get(i);
			}
		}
	else
		{
		StringId newFilename=GetPixieFilename(images.Get(0).filename,true,".pix");
		printf("\n\t%s",newFilename.GetString());
		Asset asset(newFilename.GetString());
		asset.Create();
		asset.Write(header,8);
		int version=0;
		asset.Write(&version);
		int celCount=result.GetItemCount();
		asset.Write(&celCount);
		for (int i=0; i<celCount; i++)
			{
			result.Get(i)->WriteToAsset(&asset);
			delete result.Get(i);
			}
		asset.Close();
		}

	printf("\n");
	}


//*** Convert ***

template <class Type> 
void Convert(Array<FontFile>& fonts, bool dither, const char* header)
	{ 
	if (fonts.GetItemCount()==0)
		{
		return;
		}

	for (int i=0; i<fonts.GetItemCount(); i++)
		{
		printf("\n\t%s",fonts.Get(i).filename.GetString());
		for (int j=0; j<fonts.Get(i).font->GetGlyphCount(); j++)
			{
			ImageFont::Glyph* glyph=fonts.Get(i).font->GetGlyph(j);
			for (int k=0; k<glyph->imageLayers.GetItemCount(); k++)
				{
				Image* image=glyph->imageLayers.Get(k);
				glyph->bitmapLayers.Add(new Type(*image,dither));
				}
			}
		}


	printf("\nSaving fonts");
	for (int i=0; i<fonts.GetItemCount(); i++)
		{
		StringId newFilename=GetPixieFilename(fonts.Get(i).filename,false,".fnt");
		printf("\n\t%s",newFilename.GetString());
		Asset asset(newFilename.GetString());
		asset.Create();
		asset.Write(header,8);
		int version=0;
		asset.Write(&version);
		fonts.Get(i).font-> WriteToAsset(&asset);
		asset.Close();
		}

	printf("\n");
	}


//*** LoadFile ***

void LoadFile(const char* filename,	Array<ImageFile>& images, Array<FontFile>& fonts, bool convertImmediately, bool dither)
	{
	const char* extension=StrRChr(filename,'.');
	if (StrICmp(extension,".xml")==0)
		{
		printf("\tFont: %s\n",filename);
		ImageFont* font=new ImageFont;
		font->LoadFromXML(filename);
		FontFile file;
		file.font=font;
		file.filename=filename;
		fonts.Add(file);
		}
	else
		{
		printf("\tImage: %s\n",filename);
		Image image(filename);
		ImageFile file;
		file.image=image;
		file.filename=filename;
		images.Add(file);
		}

	if (!convertImmediately)
		{
		return;
		}

	// Make sure we have images to convert
	if (images.GetItemCount()<=0 && fonts.GetItemCount()<=0)
		{
		return;
		}

	if (format16bit)
		{
		printf("\nConverting to 16bit:");
		Convert<Bitmap_16bit>(images,false,dither,"PIX16BIT");
		Convert<Bitmap_16bit>(fonts,dither,"FNT16BIT");
		}
	if (format16bitAlpha)
		{
		printf("\nConverting to 16bitAlpha:");
		Convert<Bitmap_16bitAlpha>(images,false,dither,"PIX16BAL");
		Convert<Bitmap_16bitAlpha>(fonts,dither,"FNT16BAL");
		}
	if (format16bitAlphaCrop)
		{
		printf("\nConverting to 16bitAlphaCrop:");
		Convert<Bitmap_16bitAlphaCrop>(images,false,dither,"PIX16BAC");
		Convert<Bitmap_16bitAlphaCrop>(fonts,dither,"FNT16BAC");
		}
	if (formatAlpha)
		{
		printf("\nConverting to Alpha:");
		Convert<Bitmap_Alpha>(images,false,false,"PIXALPHA");
		Convert<Bitmap_Alpha>(fonts,false,"FNTALPHA");
		}
	if (formatAlphaGS)
		{
		printf("\nConverting to AlphaGS:");
		Convert<Bitmap_Alpha>(images,false,true,"PIXALPHA");
		Convert<Bitmap_Alpha>(fonts,true,"FNTALPHA");
		}
	if (formatRLE8)
		{
		printf("\nConverting to RLE8:");
		Convert<Bitmap_RLE8>(images,false,dither,"PIXRLE8B");
		Convert<Bitmap_RLE8>(fonts,dither,"FNTRLE8B");
		}
	if (formatRLE16)
		{
		printf("\nConverting to RLE16:");
		Convert<Bitmap_RLE16>(images,false,dither,"PIXRLE16");
		Convert<Bitmap_RLE16>(fonts,dither,"FNTRLE16");
		}

	images.Clear();
	fonts.Clear();
	}


//*** LoadDirectory ***

void LoadDirectory(const char* filename, Array<ImageFile>& images, Array<FontFile>& fonts, bool convertImmediately, bool dither)
	{
	Platform_FileSystem_Directory* directory=Platform::GetPlatform_FileSystem()->CreateDirectoryObject(filename);
	for (int i=0; i<directory->GetSubdirectoryCount(); i++)
		{
		char buffer[4096];
		const char* path=directory->GetPath();
		char c=path[StrLen(path)-1];
		if (c=='\\' || c=='/')
			{
			SNPrintF(buffer,4096,"%s%s",path,directory->GetSubdirectory(i));
			}
		else
			{
			SNPrintF(buffer,4096,"%s/%s",path,directory->GetSubdirectory(i));
			}
		LoadDirectory(buffer,images,fonts,convertImmediately,dither);
		}

	for (int i=0; i<directory->GetFileCount(); i++)
		{
		char buffer[4096];
		const char* path=directory->GetPath();
		char c=path[StrLen(path)-1];
		if (c=='\\' || c=='/')
			{
			SNPrintF(buffer,4096,"%s%s",path,directory->GetFile(i));
			}
		else
			{
			SNPrintF(buffer,4096,"%s/%s",path,directory->GetFile(i));
			}
		LoadFile(buffer,images,fonts,convertImmediately,dither);
		}
	}