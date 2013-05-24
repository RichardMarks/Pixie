//*** HashTableKey_Pointer.cpp ***

#include "HashTableKey_Pointer.h"

HashTableKey_Pointer::HashTableKey_Pointer():
	hash_(0),
	pointer_(0)
	{

	}


//*** Constructor ***

HashTableKey_Pointer::HashTableKey_Pointer(const void* pointer)
	{
	// No need for any complex calculations for this key type. Just use the number as is.
	hash_=(unsigned int)(((long long)pointer)>>2);
	pointer_=pointer;
	}


//*** GetType ***

StringId HashTableKey_Pointer::GetType() const
	{
	static StringId type("HashTableKey_Pointer");
	return type;
	}


//*** GetHash ***

unsigned int HashTableKey_Pointer::GetHash() const 
	{ 
	return hash_; 
	}
	
	
//*** GetInt ***

unsigned int HashTableKey_Pointer::GetInt() const
	{
	return hash_;
	}


//*** Compare ***

bool HashTableKey_Pointer::Compare(const HashTableKey* key) const
	{
	// Make sure key is of the correct type
	if (key->GetType()!=GetType())
		{
		return false;
		}

	return pointer_==(static_cast<const HashTableKey_Pointer*>(key))->pointer_;
	}
