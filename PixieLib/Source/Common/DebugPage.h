/**
 * \class	DebugPage
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __DebugPage_H__
#define __DebugPage_H__

// Includes
#include "DebugPageManager.h"
#include "StringId.h"
#include "Array.h"
#include "TCanvas.h"
#include "TRect.h"

// External classes
class TCanvas;
class TView;
class TStaticText;
class TButton;
class TCheckbox;
class TRadioButton;

// DebugPage
class DebugPage
	{
	public:
		DebugPage();
		~DebugPage();

		void SetFocus(int x, int y);
		StringId Select();

		void AddLabel(StringId id, int x, int y, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);
		void AddLabel(int x, int y, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);

		void AddButton(StringId id, StringId event, int x, int y, const char* text, int width = 0, TColor foreground = TBlack, TColor background = TGreen);
		void AddButton(StringId event, int x, int y, const char* text, int width = 0, TColor foreground = TBlack, TColor background = TGreen);

		void AddCheckboxGroup(StringId id, int x, int y, int width, int height, TColor foreground = TUndefined, TColor background = TUndefined);

		void AddCheckbox(StringId id, StringId event, StringId checkboxGroup, bool checked, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);
		void AddCheckbox(StringId event, StringId checkboxGroup, bool checked, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);

		void AddRadioButtonGroup(StringId id, int x, int y, int width, int height, TColor foreground = TUndefined, TColor background = TUndefined);

		void AddRadioButton(StringId id, StringId event, StringId radioButtonGroup, bool selected, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);
		void AddRadioButton(StringId event, StringId radioButtonGroup, bool selected, const char* text, TColor foreground = TUndefined, TColor background = TUndefined);

		void AddBox(StringId id, int x, int y, int width, int height, TColor foreground = TUndefined, TColor background = TUndefined);
		void AddBox(int x, int y, int width, int height, TColor foreground = TUndefined, TColor background = TUndefined);

		void RemoveWidget(StringId id);


	private:
		friend class DebugPageManager;

		void Record(float deltaTime);
		void Update(float deltaTime);
		void Render(TCanvas& canvas);

		TRect GetBounds();

		int GetCheckboxCount(StringId checkboxGroup);
		int GetRadioButtonCount(StringId radioButtonGroup);
		void DeselectRadioButtons(StringId radioButtonGroup);

		enum Type
			{
			Type_Label,
			Type_Button,
			Type_CheckboxGroup,
			Type_Checkbox,
			Type_RadioButtonGroup,
			Type_RadioButton,
			Type_Box,
			};

		struct Widget
			{
			StringId id;
			Type type;
			StringId group;
			StringId event;
			union
				{
				TView* view;
				TStaticText* staticText;
				TButton* button;
				TCheckbox* checkbox;
				TRadioButton* radioButton;
				TView* box;
				};			
			};

		struct Widget* FindWidget(StringId id) const;

	private:

		Array<Widget> widgets_;
	};

#endif /* __DebugPage_H__ */
