/**
 * \class	Button
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */
 
#ifndef __Button_H__
#define __Button_H__

// Includes
#include "StringId.h"
#include "Sprite.h"
#include "Array.h"
#include "Resource_BitmapStrip.h"

// Forward declares
class SpriteManager;

// Button
class Button:public Sprite
	{
	public:
		Button();
		Button(SpriteManager* spriteManager);
		virtual ~Button();

		enum State
			{
			State_Normal,
			State_Highlighted,
			State_Pressed,
			State_Disabled,
			
			States_Count,
			};

		virtual const Resource_BitmapStrip& GetBitmap() const;
		virtual const Resource_BitmapStrip& GetBitmap(State state) const;
		virtual void SetBitmap(State state, const Resource_BitmapStrip& bitmapStrip);

		virtual State GetState() const;
		virtual void SetState(State state);

		virtual bool IsEnabled() const;
		virtual void SetEnabled(bool enabled);	

		virtual StringId GetEventId() const;
		virtual void SetEventId(StringId eventId);

		virtual void* GetEventUserData() const;
		virtual void SetEventUserData(void* userData);

		virtual float GetCrossFadeTime() const;
		virtual void SetCrossFadeTime(float time = 0.0f );

	protected:
		friend class SpriteAction_ButtonStateCrossFade;

		virtual void SetStateCrossFadeAlpha(unsigned char fromAlpha, unsigned char toAlpha);

		virtual void Render(Bitmap& bitmap);
		virtual bool OnMouseOver(int x, int y, bool button, StringId& eventId, void*& userData);
		

	private:
		State state_;
		Resource_BitmapStrip bitmapStrips_[States_Count];
		bool enabled_;
		bool highlighted_;
		StringId eventId_;
		void* eventUserData_;
		float stateCrossFadeTime_;
		State stateCrossFadeFromState_;
		unsigned char stateCrossFadeFromAlpha_;
		unsigned char stateCrossFadeToAlpha_;
	};


#endif /* __Button_H__ */

