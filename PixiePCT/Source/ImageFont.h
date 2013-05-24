/**
 * \class	ImageFont
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Replace HashTable with a static array
 */
 
#ifndef __ImageFont_H__
#define __ImageFont_H__

// Includes
#include "XMLObject.h"
#include "Array.h"

// External classes
class Image;
class Bitmap;


// ImageFont
class ImageFont:XMLObject
	{
	public:
		ImageFont();
		
		~ImageFont();

		void LoadFromXML(const char* filename);
//		void LoadFromPIXIE_FNT(const char* filename);

		struct Glyph
			{
			int ascii;
			int width;
			int x1;
			int y1;
			int x2;
			int y2;
			int xoffset;
			int yoffset;
			Array<Image*> imageLayers;
			Array<Bitmap*> bitmapLayers;
			};

		int GetGlyphCount();
		Glyph* GetGlyph(int index);

		 
		struct Kerning
			{
			char first;
			char second;
			int amount;
			};
		
		int GetKerningCount();
		Kerning* GetKernin(int index);

		void WriteToAsset(Asset* asset);
	private:

		/// Event receiver for xml elements
		virtual XMLObject* XML_Element(StringId _name, const XMLAttributeList& _attributes);

		/// Event receiver for xml attributes
		virtual void XML_Attribute(StringId _name, const XMLVariant& _value);
	private:
		StringId name_;
		int size_;
		int glyphHeight_;
		int baseline_;
		int layerCount_;

		Array<Glyph*> glyphs_;
		Array<Kerning> kerning_;
		Array<char*> layerNames_;
	};

#endif /* __ImageFont_H__ */

