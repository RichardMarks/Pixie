
#include "Constants.h"
#include "Player.h"
#include "GameMaths.h"

Player::Player()
{
    myPositionX = 0;
    myPositionY = 0;

    myVelocityX = 0;
    myVelocityY = 0;
    
    myAngle = 0;

    mySpeed = 8.0f;

    mySprite.SetBitmap("Assets/InGame/TempPlayerSpriteSheet.pix");
    mySprite.SetOrigin(96 * 0.5f, 96 * 0.5f);
    mySprite.SetPriority(2);
    mySprite.SetPosition(constants::WINDOW_WIDTH * 0.5f, constants::WINDOW_HEIGHT * 0.5f);
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    myVelocityX *= mySpeed;
    myVelocityY *= mySpeed;
    myPositionX += myVelocityX * deltaTime;
    myPositionY += myVelocityY * deltaTime;   

    if (myPositionX > constants::WINDOW_WIDTH)
    {
        myPositionX = 0;
    }
    else if (myPositionX < 0)
    {
        myPositionX = constants::WINDOW_WIDTH;
    }

    if (myPositionY > constants::WINDOW_HEIGHT)
    {
        myPositionY = 0;
    }
    else if (myPositionY < 0)
    {
        myPositionY = constants::WINDOW_HEIGHT;
    }


    mySprite.SetCel((float)myAngle);
    //mySprite.SetPosition(myPositionX, myPositionY);
}

void Player::SetPosition(float x, float y)
{
    myPositionX = x;
    myPositionY = y;
}

float Player::GetPositionX()
{
    return myPositionX;
}

float Player::GetPositionY()
{
    return myPositionY;
}

float Player::GetVelocityX()
{
    return myVelocityX;
}

float Player::GetVelocityY()
{
    return myVelocityY;
}

void Player::SetAngle(int angle)
{
    myAngle = angle;
}

int Player::GetAngle()
{
    return myAngle;
}

void Player::Accelerate()
{
    myVelocityX += gamemaths::tables::cosLook16[myAngle];
    myVelocityY += gamemaths::tables::sinLook16[myAngle];

    float velocity = gamemaths::FastDistance2D(myVelocityX, myVelocityY);

    if (velocity >= constants::MAXIMUM_PLAYER_SPEED)
    {
        myVelocityX = (constants::MAXIMUM_PLAYER_SPEED - 1) * myVelocityX / velocity;
        myVelocityY = (constants::MAXIMUM_PLAYER_SPEED - 1) * myVelocityY / velocity;
    }
}

void Player::Drag()
{
    myVelocityX *= 0.00123f;
    myVelocityY *= 0.00123f;
}

#define YAW_DELAY 3
void Player::YawLeft()
{
    static int ylCounter = 0; if (++ylCounter < YAW_DELAY) { return; } ylCounter = 0;
    if (--myAngle < 0)
    {
        myAngle = 15;
    }
    myVelocityX = 0;
    myVelocityY = 0;
    Accelerate();
}

void Player::YawRight()
{
    static int yrCounter = 0; if (++yrCounter < YAW_DELAY) { return; } yrCounter = 0;
    if (++myAngle > 15)
    {
        myAngle = 0;
    }
    myVelocityX = 0;
    myVelocityY = 0;
    Accelerate();
}
#undef YAW_DELAY
