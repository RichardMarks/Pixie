#ifndef __TitleState_H__
#define __TitleState_H__

#include "Pixie.h"

class TitleState : public GameState, public SpriteEventListener
{
public:
    static const char* ID;
    static void Register(GameStateManager&);

private:
    static GameState* CreateInstance();

    Sprite bg;
    Sprite pixieLogo;
    Sprite heading;
    Sprite credits;

    Button startButton;
    Button continueButton;
    Button quitButton;

    TitleState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(StringId eventId, void* userData);

};

#endif // __TitleState_H__