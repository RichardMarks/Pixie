/**
 * \class	Font
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Cleanup and prioritize new format
 */
 
#ifndef __Font_H__
#define __Font_H__

// Includes
#include "XMLObject.h"
#include "Array.h"

// Forward declares
class Bitmap;
class Filename;

// Font
class Font:XMLObject
	{
	public:
		struct Character
			{
			int ascii;
			int spacing;
			int x;
			int y;
			int width;
			int height;
			int xoffset;
			int yoffset;
			bool isBlank;
			Array<Bitmap*> layers;

			Character():
				ascii(0),
				spacing(0),
				x(0),
				y(0),
				width(0),
				height(0),
				xoffset(0),
				yoffset(0),
				isBlank(false)
				{ }
			};

		Font();

		Font(
			const Asset& asset
			);

		Font(
			const Filename& filename
			);
		

		~Font();

		void Blit(
			Bitmap& bitmap, 
			int x, 
			int y, 
			const char* text, 
			int spacing = 0, 
			unsigned short modulate = 0xffff,
			unsigned char alpha = 255
			) const;

		void BlitWrap(
			Bitmap& bitmap, 
			int x, 
			int y, 
			const char* text, 
			int width,
			int hspacing = 0, 
			int vspacing = 0, 
			unsigned short modulate = 0xffff,
			unsigned char alpha = 255
			) const;


		struct Bounds
			{
			int width;
			int height;
			};

		Bounds GetBounds(
			const char* text, 
			int spacing = 0
			) const;

		Bounds GetBoundsWrap(
			const char* text, 
			int width,
			int hspacing = 0, 
			int vspacing = 0
			) const;

		int GetBaseline() const;

		int GetSize() const;

		int GetGlyphHeight() const;

		void WriteToAsset(Asset* asset);
		void ReadFromAsset(const Asset* asset);
		void ReadFromAssetNew(const Asset* asset);

		const Character& GetCharacter(int ascii) const;
	private:


		int GetKerning(int first, int second) const;


		bool WillWordFit(int xp, int maxX, const char* text, int hspacing) const;

		void LoadFont(const Filename& filename);

		/// Event receiver for xml elements
		virtual XMLObject* XML_Element(StringId _name, const XMLAttributeList& _attributes);

		/// Event receiver for xml attributes
		virtual void XML_Attribute(StringId _name, const XMLVariant& _value);

	private:
		
		struct Kerning
			{
			char first;
			char second;
			int amount;

			Kerning():
				first(0),
				second(0),
				amount(0)
				{ }

			};

		StringId name_;
		int size_;
		int lineSpacing_;
		int baseline_;
		int layerCount_;

		Character characters_[256];
		Kerning kerning_[256];

		Array<char*> layers_;	///< Used for temporary storage when reading a font from XML file
	};

#endif /* __Font_H__ */

