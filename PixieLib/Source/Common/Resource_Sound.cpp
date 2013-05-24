//*** Resource_Sound.cpp ***

#include "Resource_Sound.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "Filename.h"


//*** Constructor ***

Resource_Sound::Resource_Sound():
	sound_(0)
	{
	}


//*** Constructor ***

Resource_Sound::Resource_Sound(const Filename& filename)
	{
	sound_=siResourceManager->GetSound(filename.GetStringId());
	siResourceManager->IncreaseReferenceCount(sound_);
	}


//*** Constructor ***

Resource_Sound::Resource_Sound(const char* filenameStr)
	{
	Filename filename(filenameStr);
	sound_=siResourceManager->GetSound(filename.GetStringId());
	siResourceManager->IncreaseReferenceCount(sound_);
	}

		
//*** Constructor ***

Resource_Sound::Resource_Sound(StringId filenameId)
	{
	Filename filename(filenameId);
	sound_=siResourceManager->GetSound(filename.GetStringId());
	siResourceManager->IncreaseReferenceCount(sound_);
	}

		
//*** Destructor ***

Resource_Sound::~Resource_Sound() 
	{ 
	if (sound_)
		{
		siResourceManager->DecreaseReferenceCount(sound_);
		}
	}


///*** Copy Constructor ***

Resource_Sound::Resource_Sound(const Resource_Sound& resource):
	sound_(resource.sound_) 
	{ 
	siResourceManager->IncreaseReferenceCount(sound_);
	}


//*** Assignment operator ***

const Resource_Sound& Resource_Sound::operator =(const Resource_Sound& resource)
	{ 
	sound_=resource.sound_;
	siResourceManager->IncreaseReferenceCount(sound_);
	return *this;
	}


//*** GetSound ***

const Sound* Resource_Sound::GetSound() const
	{
	return sound_;
	}


//*** Play ***

void Resource_Sound::Play(float priority, bool looping)
	{
	if (sound_)
		{
		sound_->Play(priority,looping);
		}
	}
