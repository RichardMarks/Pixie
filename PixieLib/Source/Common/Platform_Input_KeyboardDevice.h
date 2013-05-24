/**
 * \class	Platform_Input_KeyboardDevice
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a keyboard device
 * \author	Mattias Gustavsson	
 * 
 * Provides basic access to the keyboard - both by keycode (which
 * is used for the physical keyes) and characters (which takes 
 * into account things like upper-lower case, shift/alt modifiers
 * etc).
 *
 *\todo		Documentation of the buffered input methods
 */

#ifndef __Platform_Input_KeyboardDevice_H__
#define __Platform_Input_KeyboardDevice_H__

// Includes

// Forward declares

// Platform_Input_KeyboardDevice
class Platform_Input_KeyboardDevice
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_Input_KeyboardDevice() { };
		

		// Forward declare
		enum KeyCode;	


		/** 
		 * Checks if a key with the specified key code is currently held down
		 * See the KeyCode enum for more info on key codes.
		 *
		 * \returns True if the specified key is held down, false if it isn't
		 */
		virtual bool IsKeyDown(
			KeyCode keyCode	///< The key code for the key to check
			) const = 0;
	

		/**
		 * Checks if the key, or combination of keys, representing the character
		 * with the specified ascii number is currently held down. Characters
		 * are different from keys, in that a combination of keys may be needed
		 * to produce certain characters, and in that key repeat effects may cause
		 * a character to be reported as alternately up and down, if held for a
		 * period of time.
		 *
		 * \returns	True if the character is held down, false if it isn't
		 */
		virtual bool IsCharDown(
			char ascii	///< Ascii code of the character to check
			) const = 0;


		virtual void ClearBufferedCharacters() const = 0;

		virtual int GetBufferedCharacterCount() const = 0;

		virtual char GetBufferedCharacter(
			int index
			) const = 0;

		/**
		 * Enumeration of all keys that can be tested. Currently, this is based
		 * on the windows set of keys - it is up to each platform specific 
		 * implementation to try and re-map the platforms keys as closely as 
		 * possible to the ones specified here. This list can and will be expanded
		 * as needed, when more platforms are supported by the Pixie Game Engine.
		 */
		enum KeyCode
			{
			KEY_INVALID = 0x00, ///< Not a valid key code

			// 01-02 Undefined  

			KEY_CANCEL	= 0x03, ///< Control-break processing  

			// 05-07 Undefined  

			KEY_BACK	= 0x08, ///< BACKSPACE key  
			KEY_TAB		= 0x09, ///< TAB key  

			// 0A-0B Undefined  

			KEY_CLEAR	= 0x0C, ///< CLEAR key  
			KEY_RETURN	= 0x0D, ///< ENTER key  

			// 0E-0F Undefined  

			KEY_SHIFT	= 0x10, ///< SHIFT key  
			KEY_CONTROL	= 0x11, ///< CTRL key  
			KEY_MENU	= 0x12, ///< ALT key  
			KEY_CAPITAL	= 0x14, ///< CAPS LOCK key  

			// 15-19 Reserved for Kanji systems  
			// 1A Undefined  

			KEY_ESCAPE	= 0x1B, ///< ESC key  

			// 1C-1F Reserved for Kanji systems  

			KEY_SPACE	= 0x20, ///< SPACEBAR key 
			KEY_PRIOR	= 0x21, ///< PAGE UP key  
			KEY_NEXT	= 0x22, ///< PAGE DOWN key  
			KEY_END		= 0x23, ///< END key  
			KEY_HOME	= 0x24, ///< HOME key  
			KEY_LEFT	= 0x25, ///< LEFT ARROW key  
			KEY_UP		= 0x26, ///< UP ARROW key  
			KEY_RIGHT	= 0x27, ///< RIGHT ARROW key  
			KEY_DOWN	= 0x28, ///< DOWN ARROW key  
			KEY_SELECT	= 0x29, ///< SELECT key  

			// 2A Original equipment manufacturer (OEM) specific  

			KEY_EXEC	= 0x2B, ///< EXECUTE key  
			KEY_SNAPSHOT= 0x2C, ///< PRINT SCREEN key for Windows 3.0 and later  
			KEY_HELP	= 0x2F, ///< HELP key  
			KEY_0		= 0x30, ///< 0 key  
			KEY_1		= 0x31, ///< 1 key  
			KEY_2		= 0x32, ///< 2 key  
			KEY_3		= 0x33, ///< 3 key  
			KEY_4		= 0x34, ///< 4 key  
			KEY_5		= 0x35, ///< 5 key  
			KEY_6		= 0x36, ///< 6 key  
			KEY_7		= 0x37, ///< 7 key  
			KEY_8		= 0x38, ///< 8 key  
			KEY_9		= 0x39, ///< 9 key  

			// 3A-40 Undefined  

			KEY_A		= 0x41, ///< A key  
			KEY_B		= 0x42, ///< B key  
			KEY_C		= 0x43, ///< C key  
			KEY_D		= 0x44, ///< D key  
			KEY_E		= 0x45, ///< E key  
			KEY_F		= 0x46, ///< F key  
			KEY_G		= 0x47, ///< G key  
			KEY_H		= 0x48, ///< H key  
			KEY_I		= 0x49, ///< I key  
			KEY_J		= 0x4A, ///< J key  
			KEY_K		= 0x4B, ///< K key  
			KEY_L		= 0x4C, ///< L key  
			KEY_M		= 0x4D, ///< M key  
			KEY_N		= 0x4E, ///< N key  
			KEY_O		= 0x4F, ///< O key  
			KEY_P		= 0x50, ///< P key  
			KEY_Q		= 0x51, ///< Q key  
			KEY_R		= 0x52, ///< R key  
			KEY_S		= 0x53, ///< S key  
			KEY_T		= 0x54, ///< T key  
			KEY_U		= 0x55, ///< U key  
			KEY_V		= 0x56, ///< V key  
			KEY_W		= 0x57, ///< W key  
			KEY_X		= 0x58, ///< X key  
			KEY_Y		= 0x59, ///< Y key  
			KEY_Z		= 0x5A, ///< Z key  

			// 5B-5F Undefined  
			
			KEY_LWIN	= 0x5B,   
			KEY_RWIN	= 0x5C,   
			KEY_APPS	= 0x5D,   

			KEY_NUMPAD0	= 0x60, ///< Numeric keypad 0 key  
			KEY_NUMPAD1	= 0x61, ///< Numeric keypad 1 key  
			KEY_NUMPAD2	= 0x62, ///< Numeric keypad 2 key  
			KEY_NUMPAD3	= 0x63, ///< Numeric keypad 3 key  
			KEY_NUMPAD4	= 0x64, ///< Numeric keypad 4 key  
			KEY_NUMPAD5	= 0x65, ///< Numeric keypad 5 key  
			KEY_NUMPAD6	= 0x66, ///< Numeric keypad 6 key  
			KEY_NUMPAD7	= 0x67, ///< Numeric keypad 7 key  
			KEY_NUMPAD8	= 0x68, ///< Numeric keypad 8 key  
			KEY_NUMPAD9	= 0x69, ///< Numeric keypad 9 key  
			KEY_MULTIPLY= 0x6A, ///< Asterisk (*) key  
			KEY_ADD		= 0x6B, ///< Plus sign (+) key  
			KEY_SEPARATOR=0x6C, ///< Separator key  
			KEY_SUBTRACT= 0x6D, ///< Minus sign (-) key  
			KEY_DECIMAL	= 0x6E, ///< Period (.) key  
			KEY_DIVIDE	= 0x6F, ///< Slash mark (/) key  
			
			KEY_F1		= 0x70, ///< F1
			KEY_F2		= 0x71, ///< F2
			KEY_F3		= 0x72, ///< F3
			KEY_F4		= 0x73, ///< F4
			KEY_F5		= 0x74, ///< F5
			KEY_F6		= 0x75, ///< F6
			KEY_F7		= 0x76, ///< F7
			KEY_F8		= 0x77, ///< F8
			KEY_F9		= 0x78, ///< F9
			KEY_F10		= 0x79, ///< F10
			KEY_F11		= 0x7A, ///< F11
			KEY_F12		= 0x7B, ///< F12

			// 88-8F Unassigned  
			// 92-B9 Unassigned  

			KEY_LSHIFT	= 0xA0,   
			KEY_RSHIFT	= 0xA1,
			KEY_LCONTROL= 0xA2,  
			KEY_RCONTROL= 0xA3,
			KEY_LMENU	= 0xA4,  
			KEY_RMENU	= 0xA5,

			// BA-C0 OEM-specific  
			// C1-DA Unassigned  
			// DB-E4 OEM-specific  
			// E5 Unassigned  
			// E6 OEM-specific  
			// E7-E8 Unassigned  
			// E9-F5 OEM-specific  

			KEY_ATTN	= 0xF6,   
			KEY_CRSEL	= 0xF7,   
			KEY_EXSEL	= 0xF8,   
			KEY_EREOF	= 0xF9,   
			KEY_PLAY	= 0xFA,   
			KEY_ZOOM	= 0xFB,   
			KEY_NONAME	= 0xFC,
			KEY_PA1		= 0xFD,

			// FE OEM-clear
			
			KEYCODE_COUNT = 0xFF
			};
	};

#endif /* __Platform_Input_KeyboardDevice_H__ */
