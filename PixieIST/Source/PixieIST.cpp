//*** PixieIST.cpp ***

#include <cstdio>

#include "Pixie.h"
#include "PixieIST.h"

using namespace pixie;

const char* PixieIST::version = "1.0";

PixieIST::PixieIST()
{
}

PixieIST::~PixieIST()
{
}

void PixieIST::Main(const char* commandLineString)
{
    ImageFormat_PNG::Register();
	ImageFormat_JPG::Register();
	ImageFormat_GIF::Register();
	ImageFormat_TGA::Register();

    printf("Pixie Image Slicing Tool %s\n\n", PixieIST::version);

    // handle the command line
    CommandLine cmd(commandLineString);

    // PixieIST sheetfilename cellwidth cellheight
    if (cmd.GetCommandLineStringCount() < 4)
    {
        DisplayProgramUsage();
        return;
    }

    // get params
    StringId sheetFilename = cmd.GetCommandLineString(1);
    int sliceWidth = StringToInt(cmd.GetCommandLineString(2).GetString());
    int sliceHeight = StringToInt(cmd.GetCommandLineString(3).GetString());

    // validate params
    Asset sheetAsset(sheetFilename);
    if (!sheetAsset.Open())
    {
        printf("Unable to open %s\n", sheetFilename.GetString());
        return;
    }
    sheetAsset.Close();
    Image sheetImage(sheetFilename);
    int sheetWidth = sheetImage.GetWidth();
    int sheetHeight = sheetImage.GetHeight();

    if (sliceWidth <= 0)
    {
        printf("Error: Invalid Slice Width %d\n", sliceWidth);
        return;
    }

    if (sliceHeight <= 0)
    {
        printf("Error: Invalid Slice Height %d\n", sliceHeight);
        return;
    }
    
    printf(
        "Proceeding with the following settings:\n"
        "    sheet: %s (%dx%d)\n"
        "    slice width: %d\n"
        "    slice height: %d\n\n", 
        sheetFilename.GetString(),
        sheetWidth,
        sheetHeight,
        sliceWidth,
        sliceHeight);

    int xSlices = sheetWidth / sliceWidth;
    int ySlices = sheetHeight / sliceHeight;
    int sliceCount = xSlices * ySlices;

    printf("Slicing %s into %d %dx%d images...\n", sheetFilename.GetString(), sliceCount, sliceWidth, sliceHeight);

    int baseLen = StrLen(sheetFilename.GetString()) - 4;
    char* baseName = new char[baseLen + 1];
    StrNCpy(baseName, sheetFilename.GetString(), baseLen);
    baseName[baseLen] = 0;

    char countBuffer[5];
    for (int i = 0; i < sliceCount; i++)
    {
        SNPrintF(countBuffer, 5, "%04d", (i + 1));
        char* sliceName = new char[StrLen(countBuffer) + StrLen(baseName) + 7];
        StrCpy(sliceName, baseName);
        StrCat(sliceName, "_");
        StrCat(sliceName, countBuffer);
        StrCat(sliceName, ".tga");

        Image sliceImage(sliceWidth, sliceHeight);

        // copy pixel data
        int fromX = sliceWidth * (i % xSlices);
        int fromY = sliceHeight * (i / xSlices);
        for (int y = 0; y < sliceHeight; y++)
        {
            int y2 = fromY + y;
            for (int x = 0; x < sliceWidth; x++)
            {
                unsigned pixel = sheetImage.GetPixel(fromX + x, y2);
                sliceImage.SetPixel(x, y, pixel);
            }
        }
        //printf("*** Slicing from %6d, %6d\n", fromX, fromY);

        printf("  Saving %s\n", sliceName);
        sliceImage.Save<ImageFormat_TGA>(sliceName);
        delete [] sliceName;
    }

    delete [] baseName;
}


void PixieIST::DisplayProgramUsage()
{
    printf(
        "Usage Syntax:\n"
        "    PixieIST sheet width height\n\n"
        "        sheet     : Specifies the filename of the image to slice\n"
        "        width     : Specifies the width of a single slice in pixels\n"
        "        height    : Specifies the height of a single slice in pixels\n\n");
}
