//*** DebugPageManager.cpp ***

#include "DebugPageManager.h"
#include "DebugPage.h"
#include "InputManager.h"
#include "Bitmap.h"
#include "TDraw.h"
#include "SystemFont.h"

namespace pixie {
//*** Constructor ***

DebugPageManager::DebugPageManager():
	canvas_(0),
	background_(TBlack),
	backgroundAlpha_(255),
	screenWidth_(0),
	screenHeight_(0),
	isShowingDebugPage_(false),
	closeX_(0),
	closeY_(0)
	{
	SetBackground();
	}


//*** Destructor ***

DebugPageManager::~DebugPageManager()
	{
	for (int i=0; i<pages_.GetItemCount(); i++)
		{
		DebugPageEntry& entry = pages_.Get(i);

		void* parameters[1] = { entry.page };
		entry.handler(DebugPageCommand_Destroy,entry.instance,parameters);

		delete entry.page;
		}

	if (canvas_)
		{
		delete canvas_;
		canvas_ = 0;
		}
	}


//*** SetBackground ***

void DebugPageManager::SetBackground(TColor background, unsigned char alpha)
	{
	background_ = background;
	backgroundAlpha_ = alpha;
	if (canvas_)
		{
		canvas_->SetBackgroundAlpha(alpha);
		}
	}


//*** Update ***

void DebugPageManager::Update(float deltaTime)
	{
	StringId originalDebugPage = currentDebugPage_;
	bool wasShowingDebugPage = isShowingDebugPage_;

	bool isCtrlDown = siInputManager->IsKeyDown(KEY_LCONTROL) | siInputManager->IsKeyDown(KEY_RCONTROL);

	if (siInputManager->WasKeyPressed(KEY_HOME) && isCtrlDown)
		{
		isShowingDebugPage_=!isShowingDebugPage_;
		if (isShowingDebugPage_ && currentDebugPage_==StringId() && pages_.GetItemCount()>0)
			{
			currentDebugPage_=pages_.Get(0).id;
			}
		}

	if (isShowingDebugPage_ && currentDebugPage_!=StringId() && canvas_)
		{
		int xp = (int)siInputManager->GetCursorX();
		int yp = (int)siInputManager->GetCursorY();
		xp = (xp - offsetX_)/(canvas_->GetFont()==TFont8x8?8:9);
		yp = (yp - offsetY_)/(canvas_->GetFont()==TFont8x8?8:16);
		bool close = siInputManager->WasKeyPressed(KEY_LBUTTON);
		close |= siInputManager->WasKeyPressed(KEY_LCONTROL);
		close &= xp==closeX_;
		close &= yp==closeY_;
		if (close)
			{
			isShowingDebugPage_=false;
			currentDebugPage_=StringId();
			}
		}

	if (siInputManager->WasKeyPressed(KEY_END) && isCtrlDown)
		{
		// Code to display list of available debug pages, for selection
		}

	if (pages_.GetItemCount()>0)
		{
		if (siInputManager->WasKeyPressed(KEY_PRIOR) && isCtrlDown)
			{
			if (currentDebugPage_==StringId())
				{
				isShowingDebugPage_ = true;
				currentDebugPage_ = pages_.Get(pages_.GetItemCount()-1).id;
				}
			else
				{
				for (int i=0; i<pages_.GetItemCount(); i++)
					{
					DebugPageEntry& entry = pages_.Get(i);

					if (entry.id==currentDebugPage_)
						{
						if (i<=0)
							{
							isShowingDebugPage_ = false;
							currentDebugPage_ = StringId();
							}
						else
							{
							currentDebugPage_ = pages_.Get(i-1).id;
							break;
							}
						}
					}
				}
			}
		if (siInputManager->WasKeyPressed(KEY_NEXT) && isCtrlDown)
			{
			if (currentDebugPage_==StringId())
				{
				isShowingDebugPage_ = true;
				currentDebugPage_ = pages_.Get(0).id;
				}
			else
				{
				for (int i=0; i<pages_.GetItemCount(); i++)
					{
					DebugPageEntry& entry = pages_.Get(i);

					if (entry.id==currentDebugPage_)
						{
						if (i>=pages_.GetItemCount()-1)
							{
							isShowingDebugPage_ = false;
							currentDebugPage_ = StringId();
							}
						else
							{
							currentDebugPage_ = pages_.Get(i+1).id;
							break;
							}
						}
					}
				}
			}
		}

	for (int i=0; i<pages_.GetItemCount(); i++)
		{
		DebugPageEntry& entry = pages_.Get(i);

		void* parameters[2] = { entry.page, &deltaTime };
		entry.handler(DebugPageCommand_Record,entry.instance,parameters);

		entry.page->Record(deltaTime);
		}

	bool enteredPage = false;
	enteredPage |= isShowingDebugPage_ && !wasShowingDebugPage && currentDebugPage_!=StringId();
	enteredPage |= isShowingDebugPage_ && originalDebugPage!=currentDebugPage_ && currentDebugPage_!=StringId();
	bool exitedPage = false;
	exitedPage |= !isShowingDebugPage_ && wasShowingDebugPage && originalDebugPage!=StringId();
	exitedPage |= originalDebugPage!=currentDebugPage_ && originalDebugPage!=StringId();
	if (exitedPage)
		{
		for (int i=0; i<pages_.GetItemCount(); i++)
			{
			DebugPageEntry& entry = pages_.Get(i);

			if (entry.id==originalDebugPage)
				{
				void* parameters[2] = { entry.page };
				entry.handler(DebugPageCommand_Exit,entry.instance,parameters);
				}
			}
		}

	if (enteredPage)
		{
		for (int i=0; i<pages_.GetItemCount(); i++)
			{
			DebugPageEntry& entry = pages_.Get(i);

			if (entry.id==currentDebugPage_)
				{
				void* parameters[2] = { entry.page };
				entry.handler(DebugPageCommand_Enter,entry.instance,parameters);
				}
			}
		}

	if (isShowingDebugPage_ && currentDebugPage_!=StringId())
		{
		for (int i=0; i<pages_.GetItemCount(); i++)
			{
			DebugPageEntry& entry = pages_.Get(i);

			if (entry.id==currentDebugPage_)
				{
				if (canvas_)
					{
					int xp = (int)siInputManager->GetCursorX();
					int yp = (int)siInputManager->GetCursorY();
					xp = (xp - offsetX_)/(canvas_->GetFont()==TFont8x8?8:9);
					yp = (yp - offsetY_)/(canvas_->GetFont()==TFont8x8?8:16);
					entry.page->SetFocus(xp,yp);
					}

				bool select = siInputManager->WasKeyPressed(KEY_LBUTTON);
				select |= siInputManager->WasKeyPressed(KEY_LCONTROL);

				if (select)
					{
					StringId event =entry.page->Select();
					if (event!=StringId())
						{
						void* parameters[2] = { entry.page, &event};
						entry.handler(DebugPageCommand_Event,entry.instance,parameters);
						}
					}

				void* parameters[2] = { entry.page, &deltaTime };
				entry.handler(DebugPageCommand_Update,entry.instance,parameters);

				entry.page->Update(deltaTime);
				}
			}
		}
	}


//*** DrawCanvas ***

void DebugPageManager::DrawCanvas(Bitmap& bitmap, TCanvas& canvas) const
	{
	const unsigned short* cells = canvas.GetData();
	for (int y=0; y<canvas.GetHeight(); y++)
		{
		for (int x=0; x<canvas.GetWidth(); x++)
			{
			TCell cell = canvas.ToCell(*cells);
			if (cell.ascii!=0)
				{
				int xp = offsetX_ + x*(canvas.GetFont()==TFont8x8?8:9);
				int yp = offsetY_ + y*(canvas.GetFont()==TFont8x8?8:16);

				unsigned short bgColor = TColorToRGB565(cell.background);
				unsigned short fgColor = TColorToRGB565(cell.foreground);
				bitmap.Fill(xp,yp,xp+(canvas.GetFont()==TFont8x8?7:8),yp+(canvas.GetFont()==TFont8x8?7:15),bgColor,backgroundAlpha_);
				char c[2] = { cell.ascii, 0 };
				if (canvas.GetFont()==TFont8x8)
					{
					SystemFont8x8(bitmap.GetColorData(),bitmap.GetHPitch(),bitmap.GetVPitch(),xp,yp,c,fgColor);
					}
				else
					{
					SystemFont9x16(bitmap.GetColorData(),bitmap.GetHPitch(),bitmap.GetVPitch(),xp,yp,c,fgColor);
					}
				}
			cells++;
			}
		}
	}


//*** Render ***

void DebugPageManager::Render(Bitmap& bitmap)
	{
	if (!isShowingDebugPage_ || currentDebugPage_==StringId())
		{
		return;
		}

	if (screenWidth_!=bitmap.GetWidth() || screenHeight_!=bitmap.GetHeight())
		{
		screenWidth_=bitmap.GetWidth();
		screenHeight_=bitmap.GetHeight();
		if (canvas_)
			{
			delete canvas_;
			canvas_ = 0;
			}
		if (screenWidth_>0 && screenHeight_>0)
			{
			canvas_ = new TCanvas(TFont9x16, screenWidth_/9, screenHeight_/16);
			canvas_->SetBackgroundAlpha(backgroundAlpha_);
			}
		return;
		}

	if (screenWidth_==0 || screenHeight_ ==0 || !canvas_)
		{
		return;
		}

	canvas_->Clear();

	int currentDebugPageNumber = 0;
	for (int i=0; i<pages_.GetItemCount(); i++)
		{
		DebugPageEntry& entry = pages_.Get(i);
		if (entry.id==currentDebugPage_)
			{
			currentDebugPageNumber = i;

			TRect bounds = entry.page->GetBounds();
			bounds.Grow(2,2);

			bounds.w_ = Max((int)StrLen(currentDebugPage_.GetString())+6+8,bounds.w_);

			bounds = bounds.Intersect(canvas_->GetExtents());

			int xp = (int)siInputManager->GetCursorX();
			int yp = (int)siInputManager->GetCursorY();
			xp = (xp - offsetX_)/(canvas_->GetFont()==TFont8x8?8:9);
			yp = (yp - offsetY_)/(canvas_->GetFont()==TFont8x8?8:16);

			canvas_->SetCursor(bounds.x_,bounds.y_);
			TDraw::DoubleFrame(*canvas_,bounds.w_,bounds.h_, TWhite, background_);
			canvas_->SetCursor(bounds.x_+2,bounds.y_);
			closeX_=bounds.x_+3;
			closeY_=bounds.y_;
			TDraw::Text(*canvas_,"[~\xfe~]",TUndefined,TUndefined,(xp==closeX_&&yp==closeY_)?TRed:TLightGreen);
			int tw = StrLen(currentDebugPage_.GetString());
			int x = 1+bounds.x_+Max(0,bounds.w_-tw)/2;
			canvas_->SetCursor(x,bounds.y_);
			TDraw::Text(*canvas_,currentDebugPage_.GetString());
			canvas_->SetCursor(x-1,bounds.y_);
			TDraw::Text(*canvas_," ");
			canvas_->SetCursor(x+tw,bounds.y_);
			TDraw::Text(*canvas_," ");
			canvas_->SetCursor(bounds.x_,bounds.y_);
			TDraw::Shadow(*canvas_,bounds.w_,bounds.h_,' ',TUndefined,TBlack);
			bounds.Grow(-1,-1);
			canvas_->SetCursor(bounds.x_,bounds.y_);
			TDraw::Rect(*canvas_,bounds.w_,bounds.h_,' ', TBlack, background_);

			entry.page->Render(*canvas_);

			void* parameters[2] = { entry.page, canvas_ };
			entry.handler(DebugPageCommand_Render,entry.instance,parameters);
			}
		}

	//canvas_->SetCursor(0,0);
	//TDraw::HLine(*canvas_,canvas_->GetWidth(),' ',TBlack, TLightGrey);

	char buffer[80];

	//SNPrintF(buffer,80,"    Debug Page");
	//canvas_->SetCursor(0,0);
	//TDraw::Text(*canvas_,buffer);
	//
	//SNPrintF(buffer,80,"%s",currentDebugPage_.GetString());
	//canvas_->SetCursor((canvas_->GetWidth()-StrLen(buffer))/2,0);
	//TDraw::Text(*canvas_,buffer);


	canvas_->SetCursor(0,-1);
	TDraw::HLine(*canvas_,canvas_->GetWidth(),' ',TBlack, TLightGrey);

	canvas_->SetCursor(0,-1);
	TDraw::Text(*canvas_," ~Ctl-Home~ Hide/Show  ~Ctl-PgUp~ Previous  ~Ctl-PgDn~ Next  ~Ctl-End~ List",TUndefined,TUndefined,TRed);

	SNPrintF(buffer,80,"%d/%d    ",currentDebugPageNumber+1,pages_.GetItemCount());
	canvas_->SetCursor(-(int)StrLen(buffer),-1);
	TDraw::Text(*canvas_,buffer);

	offsetX_ = (bitmap.GetWidth()-canvas_->GetWidth()*(canvas_->GetFont()==TFont8x8?8:9))/2;
	offsetY_ = (bitmap.GetHeight()-canvas_->GetHeight()*(canvas_->GetFont()==TFont8x8?8:16))/2;
	DrawCanvas(bitmap,*canvas_);
	}


//*** Register ***

void DebugPageManager::Register(StringId id, DebugPageHandlerFunction handler, void* instance)
	{
	DebugPageEntry& entry = pages_.Add(DebugPageEntry());
	entry.id = id;
	entry.handler = handler;
	entry.instance = instance;
	entry.page = new DebugPage();

	void* parameters[1] = { entry.page };
	entry.handler(DebugPageCommand_Create,entry.instance,parameters);
	}


//*** Unregister ***

void DebugPageManager::Unregister(StringId id, DebugPageHandlerFunction handler, void* instance)
	{
	for (int i=0; i<pages_.GetItemCount(); i++)
		{
		DebugPageEntry& entry = pages_.Get(i);
		if (entry.id==id && entry.handler==handler && entry.instance==instance)
			{
			void* parameters[1] = { entry.page };
			entry.handler(DebugPageCommand_Destroy,entry.instance,parameters);

			delete entry.page;
			pages_.Remove(i);
			return;
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TestManager::DebugPageCreate(DebugPage& page)
	{
	page.AddLabel(4,4,"Test of a label");
	page.AddLabel("TestLabel",4,5,"Test of a named label");
	page.RemoveWidget("TestLabel");
	page.AddLabel("TestLabel",4,5,"Test of a named label");
	page.AddButton("TestButton",20,16,"Ok");
	page.AddCheckboxGroup("Check",4,8,0,0,TBlack,TCyan);
	page.AddCheckbox("OptionA","Check",true,"~T~his is a checkbox");
	page.AddCheckbox("OptionB","Check",false,"H~e~re is another one");
	page.AddCheckbox("OptionC","Check",true,"~A~nd the last one");
	page.AddRadioButtonGroup("Radio",4,14,0,0,TBlack,TCyan);
	page.AddRadioButton("Option1","Radio",true,"Option 1");
	page.AddRadioButton("Option2","Radio",true,"Option 2");
	page.AddRadioButton("Option3","Radio",false,"Option 3");
	}

void TestManager::DebugPageDestroy(DebugPage& page)
	{

	}

void TestManager::DebugPageEnter(DebugPage& page)
	{

	}

void TestManager::DebugPageExit(DebugPage& page)
	{

	}

void TestManager::DebugPageUpdate(DebugPage& page, float deltaTime)
	{

	}

void TestManager::DebugPageRecord(DebugPage& page, float deltaTime)
	{

	}

void TestManager::DebugPageRender(DebugPage& page, TCanvas& canvas)
	{

	}

void TestManager::DebugPageEvent(DebugPage& page, StringId eventId)
	{

	}

}; // namespace pixie
