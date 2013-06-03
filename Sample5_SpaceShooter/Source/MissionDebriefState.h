#ifndef __MissionDebriefState_H__
#define __MissionDebriefState_H__

#include "Pixie.h"

class MissionDebriefState : public pixie::GameState, public pixie::SpriteEventListener
{
public:
    static const char* ID;
    static void Register(pixie::GameStateManager&);

private:
    static pixie::GameState* CreateInstance();

    pixie::Sprite bg;
    pixie::Sprite heading;
    pixie::Sprite missionBox;
    pixie::StaticText missionText;

    pixie::Button declineButton;
    pixie::Button acceptButton;

    MissionDebriefState();
    virtual void Update(float deltaTime);

    virtual void OnSpriteEvent(pixie::StringId eventId, void* userData);

};

#endif // __MissionDebriefState_H__