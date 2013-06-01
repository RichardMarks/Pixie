/**
 * \class	SpriteManager
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */
 
#ifndef __SpriteManager_H__
#define __SpriteManager_H__

// Includes
#include "Array.h"
#include "StringId.h"

// Forward declares
class Bitmap;
class Sprite;
class SpriteEventListener;

// SpriteManager
class SpriteManager
	{
	public:	
		SpriteManager();
		~SpriteManager();

		virtual void AddSprite(Sprite* sprite);
		virtual void RemoveSprite(Sprite* sprite);
	
		virtual void Update(float deltaTime);
		virtual void Render(Bitmap& bitmap);

		virtual void AddEventListener(SpriteEventListener* listener);
		virtual void RemoveEventListener(SpriteEventListener* listener);

		virtual float GetPriority() const;
		virtual void SetPriority(float priority);

		virtual float GetOriginX() const;
		virtual float GetOriginY() const;
		virtual void SetOrigin(float x, float y);
		virtual void SetOriginX(float x);
		virtual void SetOriginY(float y);

		virtual void HighlightNext();
		virtual void HighlightPrevious();
		virtual void SelectHighlighted();
		virtual int GetHighlightedIndex();

	protected:
		void SendEvent(StringId event, void* userData);

	private:
		Sprite* FindLowestIndex();
		Sprite* FindHighestIndex();

	private:
		Array<Sprite*> sprites_;
		Array<SpriteEventListener*> eventListeners_;
		float originX_;
		float originY_;
		float priority_;
		Sprite* currentHighlighted_;
		bool activatedHighlighted_;
	};



#endif /* __SpriteManager_H__ */
