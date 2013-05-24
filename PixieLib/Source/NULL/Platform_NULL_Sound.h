/**
 * \class	Platform_NULL_Sound
 * 
 * \ingroup	platform_null
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_NULL_Sound_H__
#define __Platform_NULL_Sound_H__

// Includes
#include "Platform_Sound.h"

// Forward declares

// Platform_NULL_Sound
class Platform_NULL_Sound:public Platform_Sound
	{

	public:
		Platform_NULL_Sound();

		virtual Platform_Sound_SoundStream* CreateSoundStream(int channels, int frequency, int bitsPerSample, int size);
		
	};

#endif /* __Platform_NULL_Sound_H__ */
