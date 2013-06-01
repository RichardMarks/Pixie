//*** StringId.cpp ***/

#include "StringId.h"
#include "StringIdTable.h"

//*** Constructor ***

StringId::StringId():
	idString_(0)
	{
	}


//*** Constructor ***

StringId::StringId(const char* idString):
	idString_(0)
	{
	// Only initialize if idString is a non-zero pointer, and of non-zero length
	if (idString && idString[0])
		{
		// Do a lookup in the shared table to find this string if it already exists, or
		// to insert it into the shared table if it does not
		idString_=StringIdTable::GetInstance().FindIdString(idString);
		}
	}


//*** GetString ***

const char* StringId::GetString() const
	{
	return idString_;
	}


//*** GetHash ***

unsigned int StringId::GetHash() const
	{ 
	if (!idString_)
		{
		return 0;
		}
	
	// Hash number is stored immediately before the string
	const unsigned int* hash = reinterpret_cast<const unsigned int*>(idString_-sizeof(unsigned int));
	return *hash;
	}


//*** Copy Constructor ***

StringId::StringId(const StringId& stringId)
	{
	idString_=stringId.idString_;	
	}


//*** operator = ***

const StringId& StringId::operator=(const StringId& stringId)
	{
	idString_=stringId.idString_;
	return *this;
	}


//*** operator == ***

bool StringId::operator==(const StringId& stringId) const
	{
	return idString_==stringId.idString_;
	}


//*** operator != ***

bool StringId::operator!=(const StringId& stringId) const
	{
	return idString_!=stringId.idString_;
	}


