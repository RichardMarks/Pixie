//*** MusicManager.cpp ***

#include "MusicManager.h"
#include "Debug.h"
#include "Music.h"
#include "Asset.h"

namespace pixie {

//*** Constructor ***

MusicManager::MusicManager():
	currentMusic_(0),
	currentMusicVolume_(1),
	currentMusicLoop_(true),
	switchMusic_(0),
	switchMusicVolume_(1),
	switchMusicLoop_(true),
	state_(State_NotPlaying),
	fadeInTime_(0),
	fadeOutTime_(0),
	crossFadeTime_(0),
	currentTime_(0)
	{
	}


//*** Destructor ***

MusicManager::~MusicManager()
	{
	if (currentMusic_)
		{
		currentMusic_->Stop();
		delete currentMusic_;
		currentMusic_=0;
		}

	if (switchMusic_)
		{
		switchMusic_->Stop();
		delete switchMusic_;
		switchMusic_=0;
		}
	}


//*** Update ***

void MusicManager::Update(float deltaTime)
	{
	switch (state_)
		{
		case State_FadingIn:
			{
			currentTime_+=deltaTime;
			if (currentTime_>fadeInTime_)
				{
				currentMusic_->SetVolume(currentMusicVolume_);
				state_=State_Playing;
				return;
				}

			currentMusic_->SetVolume(currentMusicVolume_*(currentTime_/fadeInTime_));
			} break;

		case State_FadingOut:
			{
			currentTime_+=deltaTime;
			if (currentTime_>fadeOutTime_)
				{
				StopMusic(0);
				return;
				}

			currentMusic_->SetVolume(currentMusicVolume_*(1.0f-(currentTime_/fadeOutTime_)));
			} break;

		case State_SwitchFadingOut:
			{
			currentTime_+=deltaTime;
			if (currentTime_>fadeOutTime_)
				{
				StopMusic(0);
				PlayMusic(switchMusicFilename_,switchMusicLoop_,switchMusicVolume_,fadeInTime_);
				return;
				}

			currentMusic_->SetVolume(currentMusicVolume_*(1.0f-(currentTime_/fadeOutTime_)));
			} break;


		case State_CrossFading:
			{
			currentTime_+=deltaTime;
			if (currentTime_>crossFadeTime_)
				{
				currentMusic_->SetVolume(currentMusicVolume_);
				state_=State_Playing;
				if (switchMusic_)
					{
					switchMusic_->Stop();
					delete switchMusic_;
					switchMusic_=0;
					switchMusicFilename_=Filename();
					switchMusicVolume_=1;
					switchMusicLoop_=true;
					}
				return;
				}

			currentMusic_->SetVolume(currentMusicVolume_*(currentTime_/crossFadeTime_));
			switchMusic_->SetVolume(currentMusicVolume_*(1.0f-(currentTime_/crossFadeTime_)));
			} break;
		}
	}


//*** PlayMusic ***

void MusicManager::PlayMusic(const Filename& filename, bool loop, float volume, float fadeInTime)
	{
	// If we're already playing this music, we don't need to do anything
	if (state_!=State_SwitchFadingOut && currentMusicFilename_==filename && currentMusicVolume_==volume && currentMusicLoop_==loop)
		{
		if (state_==State_FadingOut)
			{
			state_=State_Playing;
			}
		return;
		}
	if (state_==State_SwitchFadingOut && switchMusicFilename_==filename && switchMusicVolume_==volume && switchMusicLoop_==loop)
		{
		return;
		}

	StopMusic(0);

	// If no fade in requested, just start the music straight away
	if (fadeInTime==0)
		{
		state_=State_Playing;
		currentMusic_=new Music(filename);
		currentMusic_->SetVolume(volume);
		currentMusic_->SetLooping(loop);
		currentMusic_->Play();
		currentMusicFilename_=filename;
		currentMusicVolume_=volume;
		currentMusicLoop_=loop;
		return;
		}

	// Start playing new music, and set it up to fade in
	currentMusic_=new Music(filename);
	currentMusic_->SetVolume(0);
	currentMusic_->SetLooping(loop);
	currentMusic_->Play();
	currentMusicFilename_=filename;
	currentMusicVolume_=volume;
	currentMusicLoop_=loop;
	state_=State_FadingIn;
	fadeInTime_=fadeInTime;
	currentTime_=0;
	}


//*** StopMusic ***

void MusicManager::StopMusic(float fadeOutTime)
	{
	// If we're not playing, don't do anything
	if (state_==State_NotPlaying && !currentMusic_ && !switchMusic_)
		{
		return;
		}

	// If no fadeout requested, just stop the music immediately
	if (fadeOutTime==0)
		{
		if (currentMusic_)
			{
			currentMusic_->Stop();
			delete currentMusic_;
			currentMusic_=0;
			currentMusicFilename_=Filename();
			currentMusicVolume_=1;
			currentMusicLoop_=true;
			}

		if (switchMusic_)
			{
			switchMusic_->Stop();
			delete switchMusic_;
			switchMusic_=0;
			switchMusicFilename_=Filename();
			switchMusicVolume_=1;
			switchMusicLoop_=true;
			}
		state_=State_NotPlaying;
		return;
		}

	// If fade out requested, set it up so it can be handled in Update (unless we're already fading out)
	if (state_!=State_FadingOut)
		{
		state_=State_FadingOut;
		fadeOutTime_=fadeOutTime;
		currentTime_=0;
		}
	}


//*** SwitchMusic ***

void MusicManager::SwitchMusic(const Filename& filename, bool loop, float volume, float fadeOutTime, float fadeInTime)
	{
	// If we're already playing this music, we don't need to do anything
	if (state_!=State_SwitchFadingOut && currentMusicFilename_==filename && currentMusicVolume_==volume && currentMusicLoop_==loop)
		{
		return;
		}
	if (state_==State_SwitchFadingOut && switchMusicFilename_==filename && switchMusicVolume_==volume && switchMusicLoop_==loop)
		{
		return;
		}


	// If no fadeout time requested, or we're not currently playing, start fading in straight away
	if (fadeOutTime==0 || currentMusic_==0)
		{
		PlayMusic(filename,loop,volume,fadeInTime);
		return;
		}

	// Set up fading out, so it can be done in the Update
	switchMusicFilename_=filename;
	switchMusicVolume_=volume;
	switchMusicLoop_=loop;
	state_=State_SwitchFadingOut;
	fadeOutTime_=fadeOutTime;
	fadeInTime_=fadeInTime;
	currentTime_=0;
	}


//*** CrossFadeMusic ***

void MusicManager::CrossFadeMusic(const Filename& filename, bool loop, float volume, float crossFadeTime)
	{
	// If we're already playing this music, we don't need to do anything
	if (state_!=State_SwitchFadingOut && currentMusicFilename_==filename && currentMusicVolume_==volume && currentMusicLoop_==loop)
		{
		return;
		}
	if (state_==State_SwitchFadingOut && switchMusicFilename_==filename && switchMusicVolume_==volume && switchMusicLoop_==loop)
		{
		return;
		}


	// If we're not playing any music, or no crossfade time requested, we can just fade this in straight away
	if (currentMusic_==0 || crossFadeTime==0)
		{
		PlayMusic(filename,loop,volume,crossFadeTime);
		return;
		}


	// Start playing new music, and set it up to fade in
	switchMusic_=currentMusic_;
	switchMusicFilename_=currentMusicFilename_;
	switchMusicVolume_=currentMusicVolume_;
	switchMusicLoop_=currentMusicLoop_;
	currentMusic_=new Music(filename);
	currentMusic_->SetVolume(0);
	currentMusic_->SetLooping(loop);
	currentMusic_->Play();
	currentMusicFilename_=filename;
	currentMusicVolume_=volume;
	currentMusicLoop_=loop;
	state_=State_CrossFading;
	crossFadeTime_=crossFadeTime;
	currentTime_=0;
	}


//*** GetCurrentMusicFilename ***

const Filename& MusicManager::GetCurrentMusicFilename()
	{
	return currentMusicFilename_;
	}


//*** GetCurrentMusic ***

Music* MusicManager::GetCurrentMusic()
	{
	return currentMusic_;
	}

}; // namespace pixie
