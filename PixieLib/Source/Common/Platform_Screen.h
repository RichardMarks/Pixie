/**
 * \class	Platform_Screen
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of the display device
 * \author	Mattias Gustavsson
 *
 * This is a platform abstraction of the physical screen, the display
 * device. This is what you use to display things, and it is really
 * quite simple: you simply call Present, passing it the pixel data you
 * want to display. The idea is that you keep drawing your sprites
 * and background to a pixel buffer you have created, and every frame you
 * call the Present method of Platform_Screen to display the results.
 *
 * Platform_Screen also includes functionality to set screen size and
 * toggle fullscreen mode. It is not guaranteed that every platform
 * makes use of this information, so consider it more of a hint than
 * an actual directive. What you DO have control over is the size of
 * your own bitmap, as that is what you will be drawing to.
 *
 * For platform specific behavior, please refer to the header file
 * of the platform specific implementation of Platform_Screen.
 */

#ifndef __Platform_Screen_H__
#define __Platform_Screen_H__

// Includes
#include "Platform.h"

// Forward declares
namespace pixie {

// Platform_Screen
class Platform_Screen
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Screen() { };


		/**
		 * Displays the specified bitmap on the screen. This is generally
		 * implemented in the fastest way possible for the platform, and
		 * the bitmap does not have to match the screen size, as it will
		 * do what it can to make it fit (stretch/shrink with or without
		 * interpolation, depending on settings and the current platform).
		 * Aspect ratio will be maintained, so there might be borders at
		 * the sides or top/bottom of the screen.
		 */
		virtual void Present(
			unsigned short* bitmapData,	///< Bitmap data (pixels) in 16-bit RGB565 format
			int bitmapWidth, ///< Width of the bitmap data, in pixels
			int bitmapHeight,///< Height of the bitmap data, in pixels
			unsigned short modulate = 0xffff,	///< Optional color modifier. Every pixel of the bitmap will be multiplied by this RGB565 color value, in the fastest way possible for the platform.
			unsigned short backgroundColor = 0x0000	///< Optional background color. In the case where the bitmap won't fit tightly on the screen, there will be borders above/below or to the left/right (or both). They will be the RGB565 color specified here
			) = 0;


		virtual void Present(
			unsigned int* bitmapData,	///< Bitmap data (pixels) in 32-bit X8R8G8B8 format
			int bitmapWidth, ///< Width of the bitmap data, in pixels
			int bitmapHeight,///< Height of the bitmap data, in pixels
			unsigned int modulate = 0xffffffff,	///< Optional color modifier. Every pixel of the bitmap will be multiplied by this X8R8G8B8 color value, in the fastest way possible for the platform.
			unsigned int backgroundColor = 0x00000000	///< Optional background color. In the case where the bitmap won't fit tightly on the screen, there will be borders above/below or to the left/right (or both). They will be the X8R8G8B8 color specified here
			) = 0;


		/**
		 * Enables or disables fullscreen mode. On some platforms, it is possible to run the game
		 * in a window, instead of having it cover the whole screen (which is the default behavior,
		 * unless you specify the command line parameter -window in which case the default will be
		 * to run in windowed mode).
		 *
		 * When the game is running in windowed mode, the values set by the SetSize method are used
		 * for the size of the window (when running in fullscreen mode, those values will be ignored,
		 * and the bitmap passed to Present will be scaled to fill the actual screen).
		 *
		 * On some platforms, it is possible for the user to change the window size when  running
		 * in windowed mode. This will be handled automatically, and the GetWidth and GetHeight
		 * methods can be used to retrieve the current window size. If you need to override the
		 * users change, you can call SetSize to adjust the window dimensions.
		 *
		 * Note that windowed mode might not be available on all platforms, and in that case, toggling this
		 * setting will have no effect.
		 */
		virtual void SetFullscreen(
			bool fullscreen	///< True if you want the game to run in fullscreen mode, false if you want it to run in windowed mode
			) = 0;


		/**
		 * This method is used to determine if the game is running in windowed or fullscreen mode.
		 * See the SetFullscreen method for an explanation of windowed and fullscreen mode.
		 *
		 * \returns	True if running in fullscreen mode, False if running in windowed mode
		 */
		virtual bool GetFullscreen() = 0;


		/**
		 * Sets the size of the window, if running in windowed mode. This method might not do anything
		 * on some platforms.
		 * See the SetFullscreen method for an explanation of windowed and fullscreen mode.
		 */
		virtual void SetSize(
			int width,	///< The desired window width in pixels
			int height	///< The desired window height in pixels
			) = 0;


		/**
		 * Retrieves the current window width, if running in windowed mode.
		 * See the SetFullscreen method for an explanation of windowed and fullscreen mode.
		 *
		 * \returns The current width of the window
		 */
		virtual int GetWidth() = 0;


		/**
		 * Retrieves the current window height, if running in windowed mode.
		 * See the SetFullscreen method for an explanation of windowed and fullscreen mode.
		 *
		 * \returns The current height of the window
		 */
		virtual int GetHeight() = 0;


		/**
		 * Enables or disables interpolation mode. If interpolation mode is on, it means that bitmaps
		 * of a size that doesn't match the screen size, and therefor needs to be scaled, will be interpolated
		 * using some interpolation scheme available on the platform (for example, it might use bilinear
		 * interpolation) which makes the image smoother, but a bit more blurry. If interpolation mode is off,
		 * there will be no interpolation applied to the bitmap, and it will also not be scaled by fractional
		 * amounts (meaning it might be scaled x2, x3, x4 etc, but not, for example, x2.5). You're likely to
		 * get larger border areas with interpolation turned off, but your pixels will stay sharp and crisp,
		 * with no distortion.
		 *
		 * In general, you want to turn off interpolation if the graphics for your game falls into the "pixel art"
		 * category, and leave it on (which is the default) for highres or rendered art.
		 *
		 * Note that interpolation might not be available on all platforms, and in that case, toggling this
		 * setting will have no effect.
		 */
		virtual void SetInterpolationMode(
			bool enabled	///< True if you want to turn on the interpolation mode, false if you want to turn it off
			) = 0;


		/**
		 * This method is used to determine if interpolation mode is currently on or off. See SetInterpolationMode
		 * method for an explanation of the interpolation mode
		 *
		 * \returns	True if interpolation mode is on, False if interpolation mode is off
		 */
		virtual bool GetInterpolationMode() = 0;


		/**
		 * When retrieving the position of the mouse cursor, you will get them in relation to the window (or
		 * the screen, if running in fullscreen mode). As Platform_Screen will do its own things (which might
		 * differ for different platform implementations) to scale, clamp or border the bitmap data passed to
		 * its Present method, it can be difficult or impossible to translate the mouse cursor position to
		 * coordinates in relation to the back buffer bitmap you're working with in your game. This method
		 * will transform the provided coordinates (as retrieved from Platform_Input_MouseDevice) to be relative
		 * to the bitmap passed in on the most recent Present call. If no Present call have been made yet, the
		 * coordinates will be left untransformed.
		 */
		virtual void TransformCursorCoordinates(
			float& x,	///< X-coordinate to transform
			float& y	///< Y-coordinate to transform
			) = 0;
	};

}; // namespace pixie

#endif /* __Platform_Screen_H__ */
