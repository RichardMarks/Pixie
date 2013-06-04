#ifndef __GameMaths_H__
#define __GameMaths_H__

namespace gamemaths
{
    namespace tables
    {
        extern float cosLook16[];

        extern float sinLook16[];
    
        void Init();
    };

    float FastDistance2D(float x, float y);
};

#endif // __GameMaths_H__