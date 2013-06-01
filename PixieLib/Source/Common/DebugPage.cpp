//*** DebugPage.cpp ***

#include "DebugPage.h"
#include "SystemFont.h"
#include "Bitmap.h"
#include "DebugPageManager.h"
#include "TDraw.h"
#include "TStaticText.h"
#include "TButton.h"
#include "TCheckbox.h"
#include "TRadioButton.h"

//** Constructor ***

DebugPage::DebugPage()
	{	
	}


//*** Destructor ***

DebugPage::~DebugPage()
	{
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.view)
			{
			delete widget.view;
			}
		}
	}


//*** Record ***

void DebugPage::Record(float deltaTime)
	{
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		}
	}


//*** SetFocus ***

void DebugPage::SetFocus(int x, int y)
	{
	for (int i=widgets_.GetItemCount()-1; i>=0; i--)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.view->SetFocus(x,y))
			{
			break;
			}
		}
	}


//*** Select ***

StringId DebugPage::Select()
	{
	for (int i=widgets_.GetItemCount()-1; i>=0; i--)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.view->HasFocus())
			{
			if (widget.type==Type_Button)
				{
				return widget.event;
				}

			if (widget.type==Type_Checkbox)
				{
				widget.checkbox->SetChecked(!widget.checkbox->IsChecked());
				return widget.event;
				}

			if (widget.type==Type_RadioButton)
				{
				if (!widget.radioButton->IsSelected())
					{
					DeselectRadioButtons(widget.group);
					widget.radioButton->SetSelected(true);
					return widget.event;
					}				
				}

			break;
			}
		}

	return StringId();
	}


//*** Update ***

void DebugPage::Update(float deltaTime)
	{
	//canvas_.SetCursor(2,2);
	//TDraw::Text(canvas_,"Test of debug renderer");
	//canvas_.SetCursor(10,10);
	//TDraw::DoubleFrame(canvas_,10,10,TYellow,TRed);
	//canvas_.SetCursor(20,12);
	//TDraw::Frame(canvas_,20,20);
	}


//*** Render ***

void DebugPage::Render(TCanvas& canvas)
	{
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.view)
			{
			widget.view->Draw(canvas);
			}
		}

	}


//*** AddLabel ***

void DebugPage::AddLabel(StringId id, int x, int y, const char* text, TColor foreground, TColor background)
	{
	Widget& widget = widgets_.Add();
	widget.id = id;
	widget.type = Type_Label;
	widget.staticText = new TStaticText();
	widget.staticText->SetPosition(x,y);
	widget.staticText->SetText(text);
	widget.staticText->SetForeground(foreground);
	widget.staticText->SetBackground(background);
	}


//*** AddLabel ***

void DebugPage::AddLabel(int x, int y, const char* text, TColor foreground, TColor background)
	{
	AddLabel(StringId(),x,y,text,foreground,background);
	}


//*** AddButton ***

void DebugPage::AddButton(StringId id, StringId event, int x, int y, const char* text, int width, TColor foreground, TColor background)
	{
	Widget& widget = widgets_.Add();
	widget.id = id;
	widget.type = Type_Button;
	widget.event = event;
	widget.button = new TButton();
	widget.button->SetPosition(x,y);
	widget.button->SetText(text);
	widget.button->SetSize(Max(StrLen(text)+4,width),1);
	widget.button->SetForeground(foreground);
	widget.button->SetBackground(background);
	widget.button->SetHighlight(TYellow);
	widget.button->SetFocusColor(TWhite);
	}


//*** AddButton ***

void DebugPage::AddButton(StringId event, int x, int y, const char* text, int width, TColor foreground, TColor background)
	{
	AddButton(StringId(),event,x,y,text,width,foreground,background);
	}


//*** AddCheckboxGroup ***

void DebugPage::AddCheckboxGroup(StringId id, int x, int y, int width, int height, TColor foreground, TColor background)
	{
	Widget& widget = widgets_.Add();
	widget.id = id;
	widget.type = Type_CheckboxGroup;
	widget.view = new TView();
	widget.view->SetPosition(x,y);
	widget.view->SetSize(width,height);
	widget.view->SetForeground(foreground);
	widget.view->SetBackground(background);
	}


//*** AddCheckbox ***

void DebugPage::AddCheckbox(StringId id, StringId event, StringId checkboxGroup, bool checked, const char* text, TColor foreground, TColor background)
	{
	Widget* group = FindWidget(checkboxGroup);
	if (group)
		{
		int count = GetCheckboxCount(checkboxGroup);
		int width = (text==0?0:StrLen(text))+4;
		TRect bounds = group->view->GetBounds();
		group->view->SetSize(Max(bounds.w_,width+2),Max(bounds.h_,count+1));

		Widget& widget = widgets_.Add();
		widget.id = id;
		widget.type = Type_Checkbox;
		widget.group = checkboxGroup;
		widget.event = event;
		widget.checkbox = new TCheckbox();
		widget.checkbox->SetPosition(bounds.x_+1,bounds.y_+count);
		widget.checkbox->SetText(text);
		widget.checkbox->SetSize(width,1);
		widget.checkbox->SetForeground(foreground);
		widget.checkbox->SetBackground(background);
		widget.checkbox->SetHighlight(TYellow);
		widget.checkbox->SetFocusColor(TWhite);
		widget.checkbox->SetChecked(checked);
		}
	}


//*** AddCheckbox ***

void DebugPage::AddCheckbox(StringId event, StringId checkboxGroup, bool checked, const char* text, TColor foreground, TColor background)
	{
	AddCheckbox(StringId(), event, checkboxGroup, checked, text, foreground, background);
	}


//*** AddRadioButtonGroup ***

void DebugPage::AddRadioButtonGroup(StringId id, int x, int y, int width, int height, TColor foreground, TColor background)
	{
	Widget& widget = widgets_.Add();
	widget.id = id;
	widget.type = Type_RadioButtonGroup;
	widget.view = new TView();
	widget.view->SetPosition(x,y);
	widget.view->SetSize(width,height);
	widget.view->SetForeground(foreground);
	widget.view->SetBackground(background);
	}


//*** AddRadioButton ***

void DebugPage::AddRadioButton(StringId id, StringId event, StringId radioButtonGroup, bool selected, const char* text, TColor foreground, TColor background)
	{
	Widget* group = FindWidget(radioButtonGroup);
	if (group)
		{
		int count = GetRadioButtonCount(radioButtonGroup);
		int width = (text==0?0:StrLen(text))+4;
		TRect bounds = group->view->GetBounds();
		group->view->SetSize(Max(bounds.w_,width+2),Max(bounds.h_,count+1));

		if (selected)
			{
			DeselectRadioButtons(radioButtonGroup);
			}

		Widget& widget = widgets_.Add();
		widget.id = id;
		widget.type = Type_RadioButton;
		widget.group = radioButtonGroup;
		widget.event = event;
		widget.radioButton = new TRadioButton();
		widget.radioButton->SetPosition(bounds.x_+1,bounds.y_+count);
		widget.radioButton->SetText(text);
		widget.radioButton->SetSize(width,1);
		widget.radioButton->SetForeground(foreground);
		widget.radioButton->SetBackground(background);
		widget.radioButton->SetFocusColor(TWhite);
		widget.radioButton->SetHighlight(TYellow);
		widget.radioButton->SetSelected(selected);
		}
	}


//*** AddRadioButton ***

void DebugPage::AddRadioButton(StringId event, StringId radioButtonGroup, bool selected, const char* text, TColor foreground, TColor background)
	{
	AddRadioButton(StringId(), event, radioButtonGroup, selected, text, foreground, background);
	}


//*** AddBox ***

void DebugPage::AddBox(StringId id, int x, int y, int width, int height, TColor foreground, TColor background)
	{
	Widget& widget = widgets_.Add();
	widget.id = id;
	widget.type = Type_Box;
	widget.box = new TView();
	widget.box->SetPosition(x,y);
	widget.box->SetSize(width,height);
	widget.box->SetForeground(foreground);
	widget.box->SetBackground(background);
	}


//*** AddBox ***

void DebugPage::AddBox(int x, int y, int width, int height, TColor foreground, TColor background)
	{
	AddBox(StringId(), x, y, width, height, foreground, background);
	}


//*** RemoveWidget ***

void DebugPage::RemoveWidget(StringId id)
	{
	if (id==StringId())
		{
		return;
		}

	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.id==id)
			{
			delete widget.view;
			widgets_.Remove(i);
			return;
			}
		}
	}


//*** FindWidget ***

DebugPage::Widget* DebugPage::FindWidget(StringId id) const
	{
	if (id==StringId())
		{
		return 0;
		}

	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.id==id)
			{
			return &widget;
			}
		}

	return 0;
	}


//*** GetCheckboxCount ***

int DebugPage::GetCheckboxCount(StringId checkboxGroup)
	{
	if (checkboxGroup==StringId())
		{
		return 0;
		}

	int count = 0;
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.group==checkboxGroup && widget.type==Type_Checkbox)
			{
			count++;
			}
		}

	return count;
	}


//*** GetRadioButtonCount ***

int DebugPage::GetRadioButtonCount(StringId radioButtonGroup)
	{
	if (radioButtonGroup==StringId())
		{
		return 0;
		}

	int count = 0;
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.group==radioButtonGroup && widget.type==Type_RadioButton)
			{
			count++;
			}
		}

	return count;
	}


//*** DeselectRadioButtons ***

void DebugPage::DeselectRadioButtons(StringId radioButtonGroup)
	{
	if (radioButtonGroup==StringId())
		{
		return;
		}

	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		if (widget.group==radioButtonGroup && widget.type==Type_RadioButton)
			{
			widget.radioButton->SetSelected(false);
			}
		}
	}


//*** GetBounds ***

TRect DebugPage::GetBounds()
	{
	TRect pageBounds;
	
	for (int i=0; i<widgets_.GetItemCount(); i++)
		{
		Widget& widget = widgets_.Get(i);
		TRect bounds = widget.view->GetBounds();
		pageBounds = pageBounds.Union(bounds);
		}

	return pageBounds;
	}