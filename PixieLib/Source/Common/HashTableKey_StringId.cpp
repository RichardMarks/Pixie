//*** HashTableKey_StringId.cpp ***

#include "HashTableKey_StringId.h"



HashTableKey_StringId::HashTableKey_StringId()
	{
	}


//*** Constructor ***

HashTableKey_StringId::HashTableKey_StringId(StringId stringId):
	stringId_(stringId)
	{
	}


//*** GetHash ***

unsigned int HashTableKey_StringId::GetHash() const 
	{ 
	return stringId_.GetHash(); 
	}
	

//*** GetStringId ***

StringId HashTableKey_StringId::GetStringId() const
	{
	return stringId_;
	}


//*** Compare ***

bool HashTableKey_StringId::Compare(const HashTableKey* key) const
	{
	// Make sure key is of the correct type
	if (key->GetType()!=GetType())
		{
		return false;
		}
	// The keys are only the same if the two strings are the same (two
	// different strings might have the same hash value)
	return stringId_==(static_cast<const HashTableKey_StringId*>(key))->stringId_;
	}


//*** GetType ***

StringId HashTableKey_StringId::GetType() const
	{
	static StringId type("HashTableKey_StringId");
	return type;
	}

