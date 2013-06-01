//*** StringPool.cpp ***/

#include "StringPool.h"
#include "StandardLibrary.h"
#include "BagIterator.h"

//*** Constructor ***

StringPool::StringPool(int stringMinLength, int stringStorageBlockSize)
	{

	}


//*** Destructor ***

StringPool::~StringPool()
	{
	for (int i=0; i<strings_.GetItemCount(); i++)
		{
		Free( strings_.Get(i) );
		}
	}


//*** AddString ***

const char* StringPool::GetPoolString(const char* uniqueString)
	{
	// Does the string exist already?
	for (int i=0; i<strings_.GetItemCount(); i++)
		{
		char* entry = strings_.Get(i);
		int& referenceCount = *((int*)entry);
		char* string = entry + sizeof(int);
		if (StrCmp(string,uniqueString)==0)
			{
			referenceCount++;
			return string;
			}
		}

	// String does not exist, so create it now
	char* entry = (char*)Malloc(sizeof(int)+StrLen(uniqueString)+1);
	int& referenceCount = *((int*)entry);
	char* string = entry + sizeof(int);
	referenceCount=1;
	StrCpy(string,uniqueString);
	strings_.Add(entry);
	return string;
	}


//*** GetReferenceCount ***

int StringPool::GetReferenceCount(const char* poolString)
	{
	#ifdef _DEBUG
		bool found = false;
		for (int i=0; i<strings_.GetItemCount(); i++)
			{
			char* entry = strings_.Get(i);
			char* string = entry + sizeof(int);
			if (StrCmp(string,poolString)==0)
				{
				found=true;
				}
			}
		Assert(found,"String not found in pool");
	#endif

	const char* entry = poolString - sizeof(int);
	int& referenceCount = *((int*)entry);
	return referenceCount;
	}


//*** IncreaseReferenceCount ***

void StringPool::IncreaseReferenceCount(const char* poolString)
	{
	#ifdef _DEBUG
		bool found = false;
		for (int i=0; i<strings_.GetItemCount(); i++)
			{
			char* entry = strings_.Get(i);
			char* string = entry + sizeof(int);
			if (StrCmp(string,poolString)==0)
				{
				found=true;
				}
			}
		Assert(found,"String not found in pool");
	#endif

	const char* entry = poolString - sizeof(int);
	int& referenceCount = *((int*)entry);
	Assert(referenceCount>0, "Invalid reference count");
	referenceCount++;
	}


//*** DecreaseReferenceCount ***

void StringPool::DecreaseReferenceCount(const char* poolString)
	{
	#ifdef _DEBUG
		bool found = false;
		for (int i=0; i<strings_.GetItemCount(); i++)
			{
			char* entry = strings_.Get(i);
			char* string = entry + sizeof(int);
			if (StrCmp(string,poolString)==0)
				{
				found=true;
				}
			}
		Assert(found,"String not found in pool");
	#endif

	const char* entry = poolString - sizeof(int);
	int& referenceCount = *((int*)entry);
	Assert(referenceCount>0, "Invalid reference count");
	referenceCount--;
	if (referenceCount<=0)
		{
		Free(const_cast<char*>(entry));
		BagIterator<char*> it(strings_);
		if (it.Find(const_cast<char*>(entry)))
			{
			strings_.Remove(it);
			}
		}
	}

