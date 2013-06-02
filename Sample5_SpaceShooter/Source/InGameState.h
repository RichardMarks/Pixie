#ifndef __InGameState_H__
#define __InGameState_H__

#include "Pixie.h"

class InGameState : public GameState, public SpriteEventListener
{
public:
    static const char* ID;
    static void Register(GameStateManager&);

private:
    static GameState* CreateInstance();

    pixie::Rectangle clearScreenRect;

    InGameState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(StringId eventId, void* userData);

};

#endif // __InGameState_H__