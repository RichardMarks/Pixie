/**
 * \class	MusicManager
 * 
 * \ingroup	audio
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __MusicManager_H__
#define __MusicManager_H__

// Includes
#include "Singleton.h"
#include "Filename.h"

// Forward declares
class Music;

// MusicManager
class MusicManager:public Singleton<MusicManager>
	{
	public:
		MusicManager();
		virtual ~MusicManager();	

		void PlayMusic(const Filename& filename, bool loop = true, float volume = 1, float fadeInTime_ = 0);
		void StopMusic(float fadeOutTime = 0.5f);

		void SwitchMusic(const Filename& filename, bool loop = true, float volume = 1, float fadeOutTime = 0.5f, float fadeInTime = 0);
		void CrossFadeMusic(const Filename& filename, bool loop = true, float volume = 1, float crossFadeTime = 1.0f);

		const Filename& GetCurrentMusicFilename();
		Music* GetCurrentMusic();

		void Update(float deltaTime);


	private:
		Filename currentMusicFilename_;
		Music* currentMusic_;
		float currentMusicVolume_;
		bool currentMusicLoop_;

		Filename switchMusicFilename_;
		Music* switchMusic_;
		float switchMusicVolume_;
		bool switchMusicLoop_;

		enum State
			{
			State_Playing,
			State_NotPlaying,
			State_FadingIn,
			State_FadingOut,
			State_SwitchFadingOut,
			State_CrossFading,
			};
		State state_;

		float fadeInTime_;
		float fadeOutTime_;
		float crossFadeTime_;
		float currentTime_;
	};

#define siMusicManager MusicManager::GetInstance()

#endif /* __MusicManager_H__ */

