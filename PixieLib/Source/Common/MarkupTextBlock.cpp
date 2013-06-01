//*** MarkupTextBlock.cpp ***

#include "MarkupTextBlock.h"
#include "StandardLibrary.h"
#include "Bitmap.h"
#include "Font.h"
#include "MarkupTextManager.h"


//*** Constructor ***

MarkupTextBlock::MarkupTextBlock():
	pmlString_(0),
	tokenBuffer_(0),
	currentTokenIsMarkupCode_(false),
	currentTokenPosition_(0),
	tokenStreamSize_(0)
	{
	}


//*** Destructor ***

MarkupTextBlock::~MarkupTextBlock()
	{
	if (pmlString_)
		{
		Free(pmlString_);
		pmlString_=0;
		}

	if (tokenBuffer_)
		{
		Free(tokenBuffer_);
		tokenBuffer_=0;
		}
	}


//*** Copy Constructor ***

MarkupTextBlock::MarkupTextBlock(const MarkupTextBlock& textblock):
	pmlString_(0),
	tokenBuffer_(0),
	currentTokenIsMarkupCode_(false),
	currentTokenPosition_(0),
	tokenStreamSize_(0)
	{
	if (textblock.pmlString_)
		{
		SetString(textblock.pmlString_);
		}
	}


//*** Assignment operator***

const MarkupTextBlock& MarkupTextBlock::operator=(const MarkupTextBlock& textblock)
	{
	SetString(textblock.pmlString_);
	return *this;
	}


//*** SetString ***

void MarkupTextBlock::SetString(const char* pmlString)
	{
	if (pmlString_)
		{
		Free(pmlString_);
		pmlString_=0;
		}
	
	if (tokenBuffer_)
		{
		Free(tokenBuffer_);
		tokenBuffer_=0;
		}

	if (pmlString)
		{
		pmlString_=StrDup(pmlString);
		tokenBuffer_=static_cast<char*>(Malloc(StrLen(pmlString_)+2));
		tokenBuffer_[0]=0;
		}

	ResetTokenStream();
	}


//*** AddString ***

void MarkupTextBlock::AddString(const char* pmlString)
	{
	if (!pmlString_)
		{
		if (tokenBuffer_)
			{
			Free(tokenBuffer_);
			tokenBuffer_=0;
			}

		if (pmlString)
			{
			pmlString_=StrDup(pmlString);
			tokenBuffer_=static_cast<char*>(Malloc(StrLen(pmlString_)+2));
			tokenBuffer_[0]=0;
			}
		ResetTokenStream();
		return;
		}
	
	if (pmlString)
		{
		if (tokenBuffer_)
			{
			Free(tokenBuffer_);
			tokenBuffer_=0;
			}

		char* str=static_cast<char*>(Malloc(StrLen(pmlString)+StrLen(pmlString_)+1));
		StrCpy(str,pmlString_);
		StrCat(str,pmlString);
		Free(pmlString_);
		pmlString_=str;
		tokenBuffer_=static_cast<char*>(Malloc(StrLen(pmlString_)+2));
		tokenBuffer_[0]=0;
		}

	ResetTokenStream();
	}


//*** GetString ***

const char* MarkupTextBlock::GetString() const
	{
	return pmlString_;
	}


//*** ResetTokenStream ***

void MarkupTextBlock::ResetTokenStream()
	{
	currentTokenPosition_=0;
	currentTokenIsMarkupCode_=false;
	tokenStreamSize_=0;
	if (pmlString_)
		{
		tokenStreamSize_=StrLen(pmlString_);
		}
	}


//*** GetNextToken ***

const char* MarkupTextBlock::GetNextToken()
	{
	currentTokenIsMarkupCode_=false;

	// If we're past the end of the string, we're done
	if (currentTokenPosition_>=tokenStreamSize_)
		{
		return 0;
		}

	// If there's whitespace, just skip forward
	while (pmlString_[currentTokenPosition_]<=32)
		{
		currentTokenPosition_++;
		if (currentTokenPosition_>=tokenStreamSize_)
			{
			return 0;
			}
		}

	// If this is the start of a markup code, look for its end marker
	if (pmlString_[currentTokenPosition_]=='[' || pmlString_[currentTokenPosition_]=='<')
		{
		int codeStart=currentTokenPosition_;
		while (currentTokenPosition_<tokenStreamSize_)
			{
			currentTokenPosition_++;
			char c=pmlString_[currentTokenPosition_];
			if (c==']' || c=='>')
				{
				currentTokenPosition_++;
				currentTokenIsMarkupCode_=true;
				// Copy code to buffer
				StrNCpy(tokenBuffer_,&pmlString_[codeStart],currentTokenPosition_-codeStart);
				tokenBuffer_[currentTokenPosition_-codeStart]=0;
				return tokenBuffer_;
				}
			}
		
		// No closing bracket, so treat this as a normal word
		currentTokenPosition_=codeStart;
		}

	// If not a markup code, just get the next word
	int wordStart=currentTokenPosition_;
	while (currentTokenPosition_<tokenStreamSize_)
		{
		currentTokenPosition_++;
		char c=pmlString_[currentTokenPosition_];
		if (c<=32 || c=='[' || c==']' || c=='<' || c=='>')
			{
			break;
			}
		}

	// Copy word to buffer
	StrNCpy(tokenBuffer_,&pmlString_[wordStart],Min(currentTokenPosition_-wordStart,tokenStreamSize_));
	tokenBuffer_[currentTokenPosition_-wordStart]=' ';	// End each token with a space
	tokenBuffer_[currentTokenPosition_-wordStart+1]=0;
	return tokenBuffer_;
	}


//*** Render ***

void MarkupTextBlock::Render(Bitmap& target, int x1, int y1, int x2, int y2, StringId hoverOption, StringId hoverLink)
	{
	if (!pmlString_)
		{
		return;
		}

	links_.Clear();
	options_.Clear();

	int extraVSpace=0;

	Font* font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Normal);
	unsigned short color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Normal);
	unsigned short bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Normal);
	unsigned char opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Normal);
	bool isBold=false;
	bool isItalic=false;
	bool isLink=false;
	bool isOption=false;
	bool isDisabledOption=false;
	bool isHeadline=false;
	bool isStyle=false;

	int xp=x1;
	int yp=y1;
	char idbuffer[256];
	StringId idString;
	StringId styleId;
	char varbuffer[1024];
	StringId varString;

	ResetTokenStream();
	const char* token=GetNextToken();
	while (token)
		{
		// Write the next word
		if (!currentTokenIsMarkupCode_)
			{
			Font* fontStore=font;
			unsigned short colorStore=color;
			unsigned short bgcolorStore=bgcolor;
			unsigned char opacityStore=opacity;

			if (isOption && idString==hoverOption)
				{
				font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_OptionHover);
				color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_OptionHover);
				bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_OptionHover);
				opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_OptionHover);
				}
			else if (isLink && idString==hoverLink)
				{
				font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_LinkHover);
				color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_LinkHover);
				bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_LinkHover);
				opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_LinkHover);
				}

			Font::Bounds bounds=font->GetBounds(token,0);	
			if (xp+bounds.width>x2)
				{
				xp=x1;
				yp+=bounds.height+extraVSpace;
				}

			int yskip=font->GetGlyphHeight()-font->GetSize();

			if (isOption)
				{
				InteractiveRegion region;
				region.x1=xp;
				region.y1=yp+yskip;
				region.x2=xp+bounds.width;
				region.y2=yp+bounds.height;
				region.id=idString;
				options_.Add(region);
				}
			else if (isLink)
				{
				InteractiveRegion region;
				region.x1=xp;
				region.y1=yp+yskip;
				region.x2=xp+bounds.width;
				region.y2=yp+bounds.height;
				region.id=idString;
				links_.Add(region);
				}

			if (opacity>0)
				{
				target.Fill(xp,yp+yskip,xp+bounds.width-1,yp+bounds.height-1,bgcolor,opacity);	
				}

			font->Blit(target,xp,yp,token,0,color);	
			xp+=bounds.width;

			font=fontStore;
			color=colorStore;
			bgcolor=bgcolorStore;
			opacity=opacityStore;
			}

		// Apply markup codes
		if (currentTokenIsMarkupCode_)
			{
			// [br] line breaks
			if (StrICmp(token,"[br]")==0 || StrICmp(token,"<br>")==0)
				{
				xp=x1;
				if (font)
					{
					Font::Bounds bounds=font->GetBounds(" ",0);	
					yp+=bounds.height+extraVSpace;
					}
				else
					{
					yp+=10+extraVSpace;
					}
				}

			// [var=...] horizontal spacing
			if (StrNICmp(token,"[var=",5)==0 || StrNICmp(token,"<var=",5)==0)
				{
				int l=Min(StrLen(token)-6,1023);
				StrNCpy(varbuffer,&token[5],l);
				varbuffer[l]=0;		
				varString=varbuffer;
				siMarkupTextManager->GetVariableValue(varString,varbuffer,1024);
				currentTokenIsMarkupCode_=false;
				token=varbuffer;
				continue;
				}

			// [hspace=...] horizontal spacing
			if (StrNICmp(token,"[hspace=",8)==0 || StrNICmp(token,"<hspace=",8)==0)
				{
				xp+=StringToInt(&token[8]);
				}

			// [vspace=...] vertical spacing
			if (StrNICmp(token,"[vspace=",8)==0 || StrNICmp(token,"<vspace=",8)==0)
				{
				yp+=StringToInt(&token[8]);
				}

			// [halign=...] horizontal alignment
			if (StrNICmp(token,"[halign=",8)==0 || StrNICmp(token,"<halign=",8)==0)
				{
				xp=StringToInt(&token[8]);
				}

			// [valign=...] vertical alignment
			if (StrNICmp(token,"[valign=",8)==0 || StrNICmp(token,"<valign=",8)==0)
				{
				yp=StringToInt(&token[8]);
				}

			bool fontChanged=false;

			// [style=] special font style
			if (StrNICmp(token,"[style=",7)==0 || StrNICmp(token,"<style=",7)==0)
				{
				isStyle=true;
				int l=Min(StrLen(token)-8,255);
				StrNCpy(idbuffer,&token[7],l);
				idbuffer[l]=0;				
				styleId=idbuffer;
				fontChanged=true;
				}

			// [/style] end of special font style
			if (StrICmp(token,"[/style]")==0 || StrICmp(token,"</style>")==0)
				{
				isStyle=false;
				fontChanged=true;
				}

			// [b] bold font
			if (StrICmp(token,"[b]")==0 || StrICmp(token,"<b>")==0)
				{
				isBold=true;
				fontChanged=true;
				}

			// [/b] non-bold font
			if (StrICmp(token,"[/b]")==0 || StrICmp(token,"</b>")==0)
				{
				isBold=false;
				fontChanged=true;
				}

			// [i] italic font
			if (StrICmp(token,"[i]")==0 || StrICmp(token,"<i>")==0)
				{
				isItalic=true;
				fontChanged=true;
				}

			// [/i] non-italic font
			if (StrICmp(token,"[/i]")==0 || StrICmp(token,"</i>")==0)
				{
				isItalic=false;
				fontChanged=true;
				}

			// [h] headline
			if (StrICmp(token,"[h]")==0 || StrICmp(token,"<h>")==0)
				{
				isHeadline=true;
				fontChanged=true;
				}

			// [/h] non-headline
			if (StrICmp(token,"[/h]")==0 || StrICmp(token,"</h>")==0)
				{
				isHeadline=false;
				fontChanged=true;

				// Automatic line breaks after headlines
				xp=x1;
				if (font)
					{
					Font::Bounds bounds=font->GetBounds(" ",0);	
					yp+=bounds.height+extraVSpace;
					}
				else
					{
					yp+=10;
					}
				}

			// [link=..] hyperlink
			if (StrNICmp(token,"[link=",6)==0 || StrNICmp(token,"<link=",6)==0)
				{
				isLink=true;
				fontChanged=true;
				int l=Min(StrLen(token)-7,255);
				StrNCpy(idbuffer,&token[6],l);
				idbuffer[l]=0;		
				idString=idbuffer;
				}

			// [/link] hyperlink end
			if (StrICmp(token,"[/link]")==0 || StrICmp(token,"</link>")==0)
				{
				isLink=false;
				fontChanged=true;
				}

			// [option=..] option
			if (StrNICmp(token,"[option=",8)==0 || StrNICmp(token,"<option=",8)==0)
				{
				isOption=true;
				fontChanged=true;
				int l=Min(StrLen(token)-9,255);
				StrNCpy(idbuffer,&token[8],l);
				idbuffer[l]=0;				
				idString=idbuffer;
				}

			// [/option] option end
			if (StrICmp(token,"[/option]")==0 || StrICmp(token,"</option>")==0)
				{
				isOption=false;
				fontChanged=true;
				}

			// [optiondisabled] disabled option
			if (StrICmp(token,"[optiondisabled]")==0 || StrICmp(token,"<optiondisabled>")==0)
				{
				isDisabledOption=true;
				fontChanged=true;
				}

			// [/optiondisabled] disabled option end
			if (StrICmp(token,"[/optiondisabled]")==0 || StrICmp(token,"</optiondisabled>")==0)
				{
				isDisabledOption=false;
				fontChanged=true;
				}

			// If any of the previous markup coded caused a font change///
			if (fontChanged)
				{
				if (isOption)
					{
					font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Option);
					color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Option);
					bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Option);
					opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Option);
					}
				else if (isDisabledOption)
					{
					font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_OptionDisabled);
					color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_OptionDisabled);
					bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_OptionDisabled);
					opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_OptionDisabled);
					}
				else if (isLink)
					{
					font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Link);
					color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Link);
					bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Link);
					opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Link);
					}
				else if (isHeadline)
					{
					font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Headline);
					color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Headline);
					bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Headline);
					opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Headline);
					}
				else 
					{
					if (isBold && isItalic)
						{
						font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_BoldItalic);
						color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_BoldItalic);
						bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_BoldItalic);
						opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_BoldItalic);
						}
					else if (isItalic)
						{
						font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Italic);
						color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Italic);
						bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Italic);
						opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Italic);
						}
					else if (isBold)
						{
						font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Bold);
						color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Bold);
						bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Bold);
						opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Bold);
						}
					else if (isStyle)
						{
						font=siMarkupTextManager->GetFont(styleId);
						color=siMarkupTextManager->GetFontColor(styleId);
						bgcolor=siMarkupTextManager->GetFontBgColor(styleId);
						opacity=siMarkupTextManager->GetFontBgOpacity(styleId);
						}
					else 
						{
						font=siMarkupTextManager->GetFont(MarkupTextManager::FontStyle_Normal);
						color=siMarkupTextManager->GetFontColor(MarkupTextManager::FontStyle_Normal);
						bgcolor=siMarkupTextManager->GetFontBgColor(MarkupTextManager::FontStyle_Normal);
						opacity=siMarkupTextManager->GetFontBgOpacity(MarkupTextManager::FontStyle_Normal);
						}
					}
				}
			}
		// Process next token
		token=GetNextToken();
		}

	}


//*** IsOverOption ***

StringId MarkupTextBlock::IsOverOption(int cursorX, int cursorY) const
	{
	for (int i=0; i<options_.GetItemCount(); i++)
		{
		InteractiveRegion region=options_.Get(i);
		if (cursorX>=region.x1 && cursorX<=region.x2 && cursorY>=region.y1 && cursorY<=region.y2)
			{
			return region.id;
			}
		}

	return 0;
	}


//*** IsOverLink ***

StringId MarkupTextBlock::IsOverLink(int cursorX, int cursorY) const
	{
	for (int i=0; i<links_.GetItemCount(); i++)
		{
		InteractiveRegion region=links_.Get(i);
		if (cursorX>=region.x1 && cursorX<=region.x2 && cursorY>=region.y1 && cursorY<=region.y2)
			{
			return region.id;
			}
		}

	return 0;
	}


