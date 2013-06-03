#ifndef __TitleState_H__
#define __TitleState_H__

#include "Pixie.h"

class TitleState : public pixie::GameState, public pixie::SpriteEventListener
{
public:
    static const char* ID;
    static void Register(pixie::GameStateManager&);

private:
    static pixie::GameState* CreateInstance();

    pixie::Sprite bg;
    pixie::Sprite pixieLogo;
    pixie::Sprite heading;
    pixie::Sprite credits;

    pixie::Button startButton;
    pixie::Button continueButton;
    pixie::Button quitButton;

    TitleState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(pixie::StringId eventId, void* userData);

};

#endif // __TitleState_H__