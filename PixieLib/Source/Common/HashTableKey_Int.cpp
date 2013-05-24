//*** HashTableKey_Int.cpp ***

#include "HashTableKey_Int.h"


//*** Constructor ***

HashTableKey_Int::HashTableKey_Int():
	hash_(0)
	{
	}


//*** Constructor ***

HashTableKey_Int::HashTableKey_Int(unsigned int number)
	{
	// No need for any complex calculations for this key type. Just use the number as is.
	hash_=number;
	}


//*** GetHash ***

unsigned int HashTableKey_Int::GetHash() const 
	{ 
	return hash_; 
	}
	
	
//*** GetInt ***

unsigned int HashTableKey_Int::GetInt() const
	{
	return hash_;
	}

	
//*** Compare ***

bool HashTableKey_Int::Compare(const HashTableKey* key) const
	{
	// Make sure key is of the correct type
	if (key->GetType()!=GetType())
		{
		return false;
		}

	// Since the key number and the hash are the same for this
	// sort of key, we can simply compare the hash values
	return hash_==(static_cast<const HashTableKey_Int*>(key))->hash_;
	}


//*** GetType ***

StringId HashTableKey_Int::GetType() const
	{
	static StringId type("HashTableKey_Int");
	return type;
	}

