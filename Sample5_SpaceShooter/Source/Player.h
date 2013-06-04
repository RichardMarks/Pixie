#ifndef __Player_H__
#define __Player_H__

#include "Pixie.h"

class Player
{
public:
    Player();
    ~Player();

    void Update(float deltaTime);

    void SetPosition(float x, float y);
    float GetPositionX();
    float GetPositionY();

    void SetAngle(int angle);
    int GetAngle();

    void Accelerate();
    void Drag();
    void YawLeft();
    void YawRight();

private:
    
    float myPositionX;
    float myPositionY;

    float myVelocityX;
    float myVelocityY;

    int myAngle;

    float mySpeed;

    pixie::Sprite mySprite;
};

#endif // __Player_H__
