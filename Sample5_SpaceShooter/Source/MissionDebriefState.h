#ifndef __MissionDebriefState_H__
#define __MissionDebriefState_H__

#include "Pixie.h"

class MissionDebriefState : public GameState, public SpriteEventListener
{
public:
    static const char* ID;
    static void Register(GameStateManager&);

private:
    static GameState* CreateInstance();

    Sprite bg;
    Sprite heading;
    Sprite missionBox;

    Button declineButton;
    Button acceptButton;

    MissionDebriefState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(StringId eventId, void* userData);

};

#endif // __MissionDebriefState_H__