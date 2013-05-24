/**
 * \class	MarkupTextManager
 * 
 * \ingroup	MarkupText
 * \brief	
 * \author	Mattias Gustavsson	
 *
 * 
 * \todo	Centered text (irregular backgrounds)
 * \todo	Refactor and cleanup
 */


#ifndef __MarkupTextManager_H__
#define __MarkupTextManager_H__

// Includes
#include "Singleton.h"
#include "StringId.h"
#include "Array.h"

// Forward declares
class Asset;
class Font;

// MarkupTextManager
class MarkupTextManager:public Singleton<MarkupTextManager>
	{
	public:
		MarkupTextManager();
		virtual ~MarkupTextManager();


		enum FontStyle
			{
			FontStyle_StringId,
			FontStyle_Normal,
			FontStyle_Bold,
			FontStyle_Italic,
			FontStyle_BoldItalic,
			FontStyle_Headline,
			FontStyle_Link,
			FontStyle_LinkHover,
			FontStyle_Option,
			FontStyle_OptionHover,
			FontStyle_OptionDisabled,
			};

		void SetFontStyle(
			FontStyle style, 
			const Asset& font, 
			unsigned short color
			);

		void SetFontStyle(
			FontStyle style, 
			const Asset& font, 
			unsigned short color, 
			unsigned short bgcolor,
			unsigned char opacity = 255
			);

		void SetFontStyle(
			StringId style, 
			const Asset& font, 
			unsigned short color
			);

		void SetFontStyle(
			StringId style, 
			const Asset& font, 
			unsigned short color, 
			unsigned short bgcolor,
			unsigned char opacity = 255
			);

		Font* GetFont(
			FontStyle style
			);

		unsigned short GetFontColor(
			FontStyle style
			);

		unsigned char GetFontBgOpacity(
			FontStyle style
			);

		unsigned short GetFontBgColor(
			FontStyle style
			);

		Font* GetFont(
			StringId style
			);

		unsigned short GetFontColor(
			StringId style
			);

		unsigned char GetFontBgOpacity(
			StringId style
			);

		unsigned short GetFontBgColor(
			StringId style
			);
		
		void RegisterVariable(
			StringId name, 
			int* value
			);
		
		void RegisterVariable(
			StringId name, 
			StringId* value
			);
		
		void RegisterVariable(
			StringId name, 
			Array<int>* values
			);

		void RegisterVariable(
			StringId name, 
			Array<StringId>* value
			);

		void UnregisterVariable(
			StringId name
			);

		void GetVariableValue(
			StringId name, 
			char* buffer, 
			int bufferSize
			);

		void GetVariableValue(
			StringId name, 
			int index, 
			char* buffer, 
			int bufferSize
			);

	private:
		struct FontStyleDefinition
			{
			StringId styleId;
			FontStyle style;
			Font* font;
			unsigned short color;
			unsigned char opacity;
			unsigned short bgcolor;
			};

		FontStyleDefinition* FindFontStyle(
			FontStyle style
			);

		FontStyleDefinition* FindFontStyle(
			StringId style
			);

		void RemoveFontStyle(FontStyle style);
		void AddFontStyle(FontStyle style, const Asset& font, unsigned short color, unsigned char opacity, unsigned short bgcolor);	

		void RemoveFontStyle(StringId style);
		void AddFontStyle(StringId style, const Asset& font, unsigned short color, unsigned char opacity, unsigned short bgcolor);	

		struct RegisteredVariable
			{
			StringId name;
			StringId* stringIdValue;
			int* intValue;
			Array<StringId>* stringIdArray;
			Array<int>* intArray;
			};

		const RegisteredVariable* FindRegisteredVariable(
			StringId name
			);
	private:
		Array<FontStyleDefinition> fontStyleDefinitions_;

		Array<RegisteredVariable> registeredVariables_;
	};

#define siMarkupTextManager  MarkupTextManager::GetInstance()

#endif /* __MarkupTextManager_H__ */
