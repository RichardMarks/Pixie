#ifndef __InGameState_H__
#define __InGameState_H__

#include "Pixie.h"

#include "Player.h"

struct Star
{
    pixie::Sprite sprite;
    float velocityX;
    float velocityY;
};

class InGameState : public pixie::GameState, public pixie::SpriteEventListener
{
public:
    static const char* ID;
    static void Register(pixie::GameStateManager&);

private:
    static pixie::GameState* CreateInstance();

    static void StarsRenderCallback(pixie::Bitmap&, pixie::RenderCall*, void*);

    pixie::Rectangle myClearScreenRect;

    pixie::RenderCall myStarsRenderer;
    Star* myStars;
    float myDT;

    Player myPlayer;

    InGameState();
    ~InGameState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(pixie::StringId eventId, void* userData);

};

#endif // __InGameState_H__