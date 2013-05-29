#ifndef __TitleState_H__
#define __TitleState_H__

#include "Pixie.h"

class TitleState : public GameState
{
public:
    static const char* ID;
    static void Register(GameStateManager&);

private:
    static GameState* CreateInstance();

    Sprite titleBg;

    TitleState();
    virtual void Update(float deltaTime);

};

#endif // __TitleState_H__