/**
 * \class	Resource_Sound
 * 
 * \ingroup	resources
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */
 
#ifndef __Resource_Sound_H__
#define __Resource_Sound_H__

// Includes
#include "StringId.h"
#include "Filename.h"

// Forward declares
class Sound;
class Filename;

// Resource_Sound
class Resource_Sound
	{
	public:
		Resource_Sound();

		Resource_Sound(const Filename& filename);

		Resource_Sound(const char* filename);

		Resource_Sound(StringId filename);

		~Resource_Sound();

		Resource_Sound(const Resource_Sound& resource);

		const Resource_Sound& operator =(const Resource_Sound& resource);

		const Sound* GetSound() const;

		void Play(float priority = 0, bool looping = false);

	private:
		const Sound* sound_;
	};

#endif /* __Resource_Sound_H__ */

