//*** HashTableKey_String.cpp ***

#include "HashTableKey_String.h"
#include "StandardLibrary.h"



HashTableKey_String::HashTableKey_String():
	string_(0),
	hash_(0)
	{
	}

HashTableKey_String::HashTableKey_String(const HashTableKey_String& string):
	string_(0),
	hash_(0)
	{
	if (string.string_)
		{
		string_=StrDup(string.string_);
		}
	hash_=string.hash_;
	}

const HashTableKey_String& HashTableKey_String::operator =(const HashTableKey_String& string	)
	{
	// Get rid of the copy of the string
	if (string_)
		{
		Free(string_);
		string_=0;
		}
	if (string.string_)
		{
		string_=StrDup(string.string_);
		}
	hash_=string.hash_;
	return *this;
	}


//*** Constructor ***

HashTableKey_String::HashTableKey_String(const char* string):
	string_(0),
	hash_(0)
	{
	
	// Keep a copy of the string
	if (string)
		{
		string_=StrDup(string);
		}
	
	// Calculate the hash value for the string
	hash_=CalculateHash(string);
	}


//*** Destructor ***

HashTableKey_String::~HashTableKey_String()
	{
	// Get rid of the copy of the string
	if (string_)
		{
		Free(string_);
		string_=0;
		}
	}


//*** GetHash ***

unsigned int HashTableKey_String::GetHash() const 
	{ 
	return hash_; 
	}
	

//*** GetString ***

const char* HashTableKey_String::GetString() const
	{
	return string_;
	}

//*** Compare ***

bool HashTableKey_String::Compare(const HashTableKey* key) const
	{
	// Make sure key is of the correct type
	if (key->GetType()!=GetType())
		{
		return false;
		}

	if (string_==0 || (static_cast<const HashTableKey_String*>(key))->string_==0)
		{
		if (string_==(static_cast<const HashTableKey_String*>(key))->string_)
			{
			return true;
			}
		return false;
		}
	// The keys are only the same if the two strings are the same (two
	// different strings might have the same hash value)
	return (StrICmp(string_,(static_cast<const HashTableKey_String*>(key))->string_)==0);
	}


//*** CalculateHash ***

unsigned int HashTableKey_String::CalculateHash(const char* key) const
	{
	unsigned long hash = 5381; // Seed value

	// Modify hash for each character in the string
	const char* stringData=key;
	while (*stringData)
		{
		// A little bit-manipulation magic to get a nice distribution of values
		hash = ((hash << 5) + hash) ^ ToUpper(*stringData);
		stringData++;
		}

	// Return the final hash value
	return hash;

	}
	

//*** GetType ***

StringId HashTableKey_String::GetType() const
	{
	static StringId type("HashTableKey_String");
	return type;
	}

