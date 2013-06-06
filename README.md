Pixie Game Engine v0.100
=======================

Pixie is a public domain 2D game engine for C++, created by Mattias Gustavsson.
This modified version of Pixie is being further developed and maintained by Richard Marks.
www.ccpssolutions.com

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
  + Pixie GameWizard - Code Generator
 + Pixie Samples
  + Hello World
  + Bouncing Ball
  + Multi Balls
  + Snake
  + Space Shooter (Currently Incomplete and should be ignored for now)

## Changes:

### Update 6 (Marked git tag v0.100)
 + Added New Tool Pixie GameWizard for generating your GameState source and header files instantly.
 + Added New Project to Solution which just copies the Pixie Toolset into a common Tools folder.

### Update 5
 + PixiePPT - Projects now use static linkage.
 + Added New Tool PixieIST for slicing up sprite sheets into individual image files

### Update 4 (Marked git tag v0.9g)
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

