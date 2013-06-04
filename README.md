Pixie Game Engine v0.9g
=======================
Modified version of Pixie by Richard Marks www.ccpssolutions.com

## Includes:

 + Microsoft Visual C++ 2008 Express Projects and Solutions
 + PixieLib
 + Pixie Toolset
  + PixieAGT - Pixie Archive Generation Tool
  + PixieFGT - Pixie Font Generation Tool
  + PixiePCT - Pixie Picture Conversion Tool
  + PixiePPT - Pixie Project Patch Tool
  + PixiePVT - Pixie Pix Viewer Tool (previously known as PixView)
  + PixieIST - Pixie Image Slicing Tool
 + Pixie Samples
  + Hello World
  + Bouncing Ball
  + Multi Balls
  + Snake

## Changes:

### Update 5
 + PixiePPT - Projects now use static linkage.
 + Added New Tool PixieIST for slicing up sprite sheets into individual image files

### Update 4
 + Backwards-compatibility-breaking-change: Refactored all of Pixie into the pixie namespace. 
   To get your old code to work you need to just use a using namespace pixie statement or prefix
   anything from Pixie with the pixie:: qualifier.

### Update 3
 + Merged in 'BleedingEdge' version of PixieLib from Mattias Gustavsson

### Update 2
 + PixieFGT completely refactored and now faster and has the ability to process multiple fonts via an XML configuration file. Will document the XML format soon.
 + Added New Sample (See Sample4_Snake directory)
 + (Ignore Sample5_SpaceShooter directory for now. It is incomplete.)

### Update 1
 + Native MSVC 2008 Express vcproj and sln files
 + PixieFGT updated to use Array instead of now-non-existing DynamicArray.
 + Slight modifications to PixieLib to reduce excess warnings given by MSVC 2008 Express

=======================
## Original readme.txt 

By: Mattias Gustavsson

For more information, tutorials, documentation and to download
example source code for games done with Pixie, please visit:

	www.pixieuniversity.com

To compile the source code, you need to have DirectX SDK installed.

The Pixie and source code is public domain. Pixie makes use of
some third-party code which isn't written by me, and it is not 
public domain - those parts of the code has their respective 
license specified.

/Mattias gustavsson

