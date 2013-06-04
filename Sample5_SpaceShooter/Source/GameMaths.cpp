#include "GameMaths.h"
#include "StandardLibrary.h"

float gamemaths::tables::cosLook16[16];
float gamemaths::tables::sinLook16[16];

void gamemaths::tables::Init()
{
    float PIOver2 = pixie::PI / 2.0f;
    for (int i = 0; i < 16; i++)
    {
        float angle = pixie::PI * (i * 22.5f) / 180.0f;
        tables::cosLook16[i] = -pixie::Cos(angle + PIOver2);
        tables::sinLook16[i] = -pixie::Sin(angle + PIOver2);
    }
}

float gamemaths::FastDistance2D(float x, float y)
{
    x = pixie::Abs(x);
    y = pixie::Abs(y);
    float mn = pixie::Min(x, y);
    return (x + y - (mn / 2.0f) - (mn  / 4.0f) + (mn  / 8.0f));
}
