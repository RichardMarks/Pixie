//*** MarkupTextManager.cpp ***

#include "MarkupTextManager.h"
#include "Font.h"
#include "StandardLibrary.h"


//*** Constructor ***

MarkupTextManager::MarkupTextManager()
	{
	}


//*** Destructor ***	

MarkupTextManager::~MarkupTextManager()
	{
	for (int i=0; i<fontStyleDefinitions_.GetItemCount(); i++)
		{
		FontStyleDefinition& definition=fontStyleDefinitions_.Get(i);
		delete definition.font;
		}
	}




//*** FindFontStyle ***

MarkupTextManager::FontStyleDefinition* MarkupTextManager::FindFontStyle(FontStyle style)
	{
	for (int i=0; i<fontStyleDefinitions_.GetItemCount(); i++)
		{
		FontStyleDefinition& definition=fontStyleDefinitions_.Get(i);
		if (definition.style==style)
			{
			return &definition;
			}
		}
	
	return 0;
	}


//*** FindFontStyle ***

MarkupTextManager::FontStyleDefinition* MarkupTextManager::FindFontStyle(StringId style)
	{
	for (int i=0; i<fontStyleDefinitions_.GetItemCount(); i++)
		{
		FontStyleDefinition& definition=fontStyleDefinitions_.Get(i);
		if (definition.style==FontStyle_StringId && definition.styleId==style)
			{
			return &definition;
			}
		}
	
	return 0;
	}


//*** RemoveFontStyle ***

void MarkupTextManager::RemoveFontStyle(FontStyle style)
	{
	for (int i=0; i<fontStyleDefinitions_.GetItemCount(); i++)
		{
		FontStyleDefinition& definition=fontStyleDefinitions_.Get(i);
		if (definition.style==style)
			{
			delete definition.font;
			fontStyleDefinitions_.Remove(i);
			return;
			}
		}
	}


//*** AddFontStyle ***

void MarkupTextManager::AddFontStyle(FontStyle style, const Asset& font, unsigned short color, unsigned char opacity, unsigned short bgcolor)
	{
	FontStyleDefinition definition;
	definition.style=style;
	definition.styleId=0;
	definition.font=new Font(font);
	definition.color=color;
	definition.opacity=opacity;
	definition.bgcolor=bgcolor;

	fontStyleDefinitions_.Add(definition);
	}

		
//*** RemoveFontStyle ***

void MarkupTextManager::RemoveFontStyle(StringId style)
	{
	for (int i=0; i<fontStyleDefinitions_.GetItemCount(); i++)
		{
		FontStyleDefinition& definition=fontStyleDefinitions_.Get(i);
		if (definition.style==FontStyle_StringId && definition.styleId==style)
			{
			delete definition.font;
			fontStyleDefinitions_.Remove(i);
			return;
			}
		}
	}


//*** AddFontStyle ***

void MarkupTextManager::AddFontStyle(StringId style, const Asset& font, unsigned short color, unsigned char opacity, unsigned short bgcolor)
	{
	FontStyleDefinition definition;
	definition.style=FontStyle_StringId;
	definition.styleId=style;
	definition.font=new Font(font);
	definition.color=color;
	definition.opacity=opacity;
	definition.bgcolor=bgcolor;

	fontStyleDefinitions_.Add(definition);
	}

//*** SetFontStyle ***

void MarkupTextManager::SetFontStyle(FontStyle style, const Asset& font, unsigned short color)
	{
	RemoveFontStyle(style);
	AddFontStyle(style,font,color,0,0xffff);	
	}


//*** SetFontStyle ***

void MarkupTextManager::SetFontStyle(FontStyle style, const Asset& font, unsigned short color, unsigned short bgcolor, unsigned char opacity)
	{
	RemoveFontStyle(style);
	AddFontStyle(style,font,color,opacity,bgcolor);	
	}


//*** SetFontStyle ***

void MarkupTextManager::SetFontStyle(StringId style, const Asset& font, unsigned short color)
	{
	RemoveFontStyle(style);
	AddFontStyle(style,font,color,0,0xffff);	
	}


//*** SetFontStyle ***

void MarkupTextManager::SetFontStyle(StringId style, const Asset& font, unsigned short color, unsigned short bgcolor, unsigned char opacity)
	{
	RemoveFontStyle(style);
	AddFontStyle(style,font,color,opacity,bgcolor);	
	}


//*** GetFont ***

Font* MarkupTextManager::GetFont(FontStyle style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->font;
		}

	return 0;
	}


//*** GetFontColor ***

unsigned short MarkupTextManager::GetFontColor(FontStyle style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->color;
		}

	return 0;
	}


//*** GetFontOpacity ***

unsigned char MarkupTextManager::GetFontBgOpacity(FontStyle style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->opacity;
		}

	return 0;
	}


//*** GetFontBgColor ***

unsigned short MarkupTextManager::GetFontBgColor(FontStyle style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->bgcolor;
		}

	return 0xffff;
	}


//*** GetFont ***

Font* MarkupTextManager::GetFont(StringId style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->font;
		}

	return 0;
	}


//*** GetFontColor ***

unsigned short MarkupTextManager::GetFontColor(StringId style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->color;
		}

	return 0;
	}


//*** GetFontOpacity ***

unsigned char MarkupTextManager::GetFontBgOpacity(StringId style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->opacity;
		}

	return 0;
	}


//*** GetFontBgColor ***

unsigned short MarkupTextManager::GetFontBgColor(StringId style)
	{
	FontStyleDefinition* definition=FindFontStyle(style);
	if (definition)
		{
		return definition->bgcolor;
		}

	return 0xffff;
	}


//*** FindRegisteredVariable ***

const MarkupTextManager::RegisteredVariable* MarkupTextManager::FindRegisteredVariable(StringId name)
	{
	for (int i=registeredVariables_.GetItemCount()-1; i>=0; i--)
		{
		const RegisteredVariable& variable=registeredVariables_.Get(i);
		if (variable.name==name)
			{
			return &variable;
			}
		}

	return 0;
	}


//*** RegisterVariable ***

void MarkupTextManager::RegisterVariable(StringId name, int* value)
	{
	RegisteredVariable variable;
	variable.name=name;
	variable.intValue=value;
	variable.stringIdValue=0;
	variable.intArray=0;
	variable.stringIdArray=0;
	registeredVariables_.Add(variable);
	}


//*** RegisterVariable ***

void MarkupTextManager::RegisterVariable(StringId name, StringId* value)
	{
	RegisteredVariable variable;
	variable.name=name;
	variable.intValue=0;
	variable.stringIdValue=value;
	variable.intArray=0;
	variable.stringIdArray=0;
	registeredVariables_.Add(variable);
	}


//*** RegisterVariable ***

void MarkupTextManager::RegisterVariable(StringId name, Array<int>* values)
	{
	RegisteredVariable variable;
	variable.name=name;
	variable.intValue=0;
	variable.stringIdValue=0;
	variable.intArray=values;
	variable.stringIdArray=0;
	registeredVariables_.Add(variable);
	}


//*** RegisterVariable ***

void MarkupTextManager::RegisterVariable(StringId name, Array<StringId>* values)
	{
	RegisteredVariable variable;
	variable.name=name;
	variable.intValue=0;
	variable.stringIdValue=0;
	variable.intArray=0;
	variable.stringIdArray=values;
	registeredVariables_.Add(variable);
	}


//*** UnregisterVariable ***

void MarkupTextManager::UnregisterVariable(StringId name)
	{
	for (int i=registeredVariables_.GetItemCount()-1; i>=0; i--)
		{
		const RegisteredVariable& variable=registeredVariables_.Get(i);
		if (variable.name==name)
			{
			registeredVariables_.Remove(i);
			return;
			}
		}
	}


//*** GetVariableValue ***

void MarkupTextManager::GetVariableValue(StringId name, char* buffer, int bufferSize)
	{
	StrCpy(buffer,"");

	const RegisteredVariable* variable=FindRegisteredVariable(name);
	if (!variable)
		{
		return;
		}

	if (variable->intValue)
		{
		if (*variable->intValue!=0)
			{
			SNPrintF(buffer,bufferSize,"%d",*(variable->intValue));
			}
		}
	else if (variable->stringIdValue)
		{
		if (variable->stringIdValue->GetString()!=0)
			{
			SNPrintF(buffer,bufferSize,"%s",variable->stringIdValue->GetString());
			}
		}
	}


//*** GetVariableValue ***

void MarkupTextManager::GetVariableValue(StringId name, int index, char* buffer, int bufferSize)
	{
	StrCpy(buffer,"");

	const RegisteredVariable* variable=FindRegisteredVariable(name);
	if (!variable)
		{
		return;
		}

	if (variable->intArray)
		{
		if (index>=0 && index<variable->intArray->GetItemCount() && variable->intArray->Get(index)!=0)
			{
			SNPrintF(buffer,bufferSize,"%d",variable->intArray->Get(index));
			}
		}
	else if (variable->stringIdArray)
		{
		if (index>=0 && index<variable->stringIdArray->GetItemCount() && variable->stringIdArray->Get(index)!=0)
			{
			SNPrintF(buffer,bufferSize,"%s",variable->stringIdArray->Get(index).GetString());
			}
		}
	}
