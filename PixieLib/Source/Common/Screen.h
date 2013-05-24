/**
 * \class	Screen
 * 
 * \ingroup	graphics
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */
 
#ifndef __Screen_H__
#define __Screen_H__

// Includes

// Forward declares
class Bitmap;


// Screen
class Screen
	{
	public:
		Screen(
			int width,
			int height
			);

		~Screen();

		Bitmap& GetBackBuffer();

		void Present(
			unsigned short modulate = 0xffff
			);


		void SetBackgroundColor(
			unsigned short color
			);

		unsigned short GetBackgroundColor();


		void SetFullscreen(
			bool fullscreen
			);

		bool GetFullscreen();


		void SetInterpolationMode(
			bool fullscreen
			);

		bool GetInterpolationMode();


		void SetWindowSize(
			int width, 
			int height
			);

		int GetWindowWidth();
		int GetWindowHeight();

		void MatchWindowToScreenSize();
	private:
		Bitmap* backBuffer_;
		unsigned short backgroundColor_;
	};

#endif /* __Screen_H__ */

