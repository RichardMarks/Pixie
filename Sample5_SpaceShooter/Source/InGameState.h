#ifndef __InGameState_H__
#define __InGameState_H__

#include "Pixie.h"

class InGameState : public pixie::GameState, public pixie::SpriteEventListener
{
public:
    static const char* ID;
    static void Register(pixie::GameStateManager&);

private:
    static pixie::GameState* CreateInstance();

    pixie::Rectangle clearScreenRect;
    pixie::Sprite player;

    InGameState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(pixie::StringId eventId, void* userData);

};

#endif // __InGameState_H__