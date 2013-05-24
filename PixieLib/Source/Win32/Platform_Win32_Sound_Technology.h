/**
 * \class	Platform_Win32_Sound_Technology
 * 
 * \ingroup	win32
 * \brief	
 * \author	Mattias Gustavsson	
 *
 */

#ifndef __Platform_Win32_Sound_Technology_H__
#define __Platform_Win32_Sound_Technology_H__

// Includes

// Forward declares
class Platform_Sound_SoundStream;

// Platform_Win32_Sound_Technology
class Platform_Win32_Sound_Technology
	{

	public:
		virtual ~Platform_Win32_Sound_Technology() { };

		virtual bool Setup() = 0;

		virtual void Update() = 0;

		virtual Platform_Sound_SoundStream* CreateSoundStream(
			int channels, 
			int frequency, 
			int bitsPerSample, 
			int size
			) = 0;

	private:
	};


#endif /* __Platform_Win32_Sound_Technology_H__ */
