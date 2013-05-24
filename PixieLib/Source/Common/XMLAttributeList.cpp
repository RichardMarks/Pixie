//*** XMLAttributeList.cpp ***

#include "XMLAttributeList.h"

//*** Add *** 

void XMLAttributeList::Add(StringId name, const XMLVariant& value)
	{
	Attribute& attribute=attributes_.Add(Attribute());
	attribute.name=name;
	attribute.value=value;
	}


//*** GetCount ***

int XMLAttributeList::GetCount() const
	{
	return attributes_.GetItemCount();
	}


//*** GetName ***

StringId XMLAttributeList::GetName(int index) const
	{
	return attributes_.Get(index).name;
	}


//*** GetValue ***

const XMLVariant& XMLAttributeList::GetValue(int index) const
	{
	return attributes_.Get(index).value;
	}


//*** GetValue ***

const XMLVariant& XMLAttributeList::GetValue(StringId name) const
	{
	int index=GetIndex(name);
	if (index<0)
		{
		static XMLVariant emptyValue;
		return emptyValue;
		}

	return GetValue(index);
	}


//*** GetIndex ***

int XMLAttributeList::GetIndex(StringId name) const
	{
	for (int i=0; i<attributes_.GetItemCount(); i++)
		{
		if (attributes_.Get(i).name==name)
			{
			return i;
			}
		}
	return -1;
	}

