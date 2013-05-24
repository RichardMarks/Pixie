/**
 * \class	Platform_Win32_Sound_SoundStream_WMM
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Platform_Win32_Sound_SoundStream_WMM_H__
#define __Platform_Win32_Sound_SoundStream_WMM_H__

// Includes
#include "Platform_Sound_SoundStream.h"

// Forward declares
class Platform_Win32_Sound_WMM;
struct HWAVEOUT__;

// Platform_Win32_Sound_SoundStream_WMM
class Platform_Win32_Sound_SoundStream_WMM : public Platform_Sound_SoundStream
	{
	public:
		Platform_Win32_Sound_SoundStream_WMM(
			Platform_Win32_Sound_WMM* manager,
			int channels, 
			int frequency, 
			int bitsPerSample, 
			int size
			);
		virtual ~Platform_Win32_Sound_SoundStream_WMM();

	
		virtual void Play();
		virtual void Stop();

		virtual int GetPosition();
		virtual void SetPosition(int position);

		virtual void CopySoundToBuffer(int bufferOffset, const void* soundData, int bytesToCopy);
		virtual void ClearBuffer(int bufferOffset, int bytesToClear);

		virtual int GetChannels();
		virtual int GetFrequency();
		virtual int GetBitsPerSample();
		virtual int GetSize();

		virtual void Update();

		virtual void SetVolume(float level);
		virtual float GetVolume();

	private:
		static void CALLBACK waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	
	private:
		Platform_Win32_Sound_WMM* manager_;

		static CRITICAL_SECTION waveCriticalSection_;

		void* soundBuffer_;
		int soundPosition_;

		HWAVEOUT__* hWaveOut_; 
		static const int WaveBlockSize = 8192;
		static const int WaveBlockCount = 8;
		WAVEHDR* waveBlocks_;
		int firstUsedWaveBlock_;
		int lastUsedWaveBlock_;
		void* waveBlocksBuffer_;

		bool playing_;

		int channels_;
		int frequency_;
		int bitsPerSample_;
		int size_;
	};

#endif /* __Platform_Win32_Sound_SoundStream_WMM_H__ */
