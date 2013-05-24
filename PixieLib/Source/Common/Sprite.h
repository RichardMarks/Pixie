/**
 * \class	Sprite
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *	
 * \todo	Documentation. Possibly the most important system to document, as it's used extensively all the time
 * \todo	Additive
 * \todo	Rotation
 * \todo	Scale
 * \todo	Mouseover bounding box
 */
 
#ifndef __Sprite_H__
#define __Sprite_H__

// Includes
#include "StringId.h"
#include "Resource_BitmapStrip.h"

// Forward declares
class SpriteManager;
class Image;
class Bitmap;
class BitmapStrip;

// Sprite
class Sprite
	{
	public:
		Sprite();
		Sprite(SpriteManager* spriteManager);
		virtual ~Sprite();

		virtual SpriteManager* GetSpriteManager() const;
		virtual void SetSpriteManager(SpriteManager* spriteManager);

		virtual float GetX() const;
		virtual float GetY() const;
		virtual void SetPosition(float x, float y);
		virtual void SetX(float x);
		virtual void SetY(float y);

		virtual float GetOriginX() const;
		virtual float GetOriginY() const;
		virtual void SetOrigin(float x, float y);
		virtual void SetOriginX(float x);
		virtual void SetOriginY(float y);

		virtual bool IsVisible() const;
		virtual void SetVisible(bool visible);

		virtual unsigned short GetColor() const;
		virtual void SetColor(unsigned short color);

		virtual unsigned char GetAlpha() const;
		virtual void SetAlpha(unsigned char alpha);

		virtual float GetPriority() const;
		virtual void SetPriority(float priority);

		virtual float GetCel() const;
		virtual void SetCel(float cel);

		virtual const Resource_BitmapStrip& GetBitmap() const;
		virtual void SetBitmap(const Resource_BitmapStrip& bitmapStrip);

		virtual bool PickPixel(int x, int y) const;

	protected:
		friend class SpriteManager;

		virtual void Render(Bitmap& bitmap);

		virtual bool OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData);

	private:
		SpriteManager* spriteManager_;
		float x_;
		float y_;
		float originX_;
		float originY_;
		bool visible_;
		unsigned short color_;
		unsigned char alpha_;
		float priority_;
		float cel_;
		Resource_BitmapStrip bitmapStrip_;
	};

#endif /* __Sprite_H__ */

