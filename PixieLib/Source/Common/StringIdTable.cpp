//*** StringIdTable.cpp ***/

#include "StringIdTable.h"
#include "StandardLibrary.h"


//*** GetInstance ***

StringIdTable& StringIdTable::GetInstance()
	{
	// There is only one instance of StringIdTable ever created, and this is the one.
	// It is declared as a static member of the GetInstance method, which makes sure
	// it is instantiated only once, the first time it is used. The StringIdTable class
	// have private constructor/destructor, and private copy and assignment operators,
	// so it can not be created or duplicated. The only way to get hold of an instance
	// of the StringIdTable is by using this method, and this guarantees that there will
	// only ever be one single instance of the engine.
	// Note that the implementation of this method may NOT be moved to a .inl file,
	// because that means idTableInstance will be inlined in several places, and we
	// will end up with more than one instance, which will really mess things up!
	static StringIdTable idTableInstance;
	return idTableInstance;
	}


//*** Constructor ***

StringIdTable::StringIdTable():
	stringStorageBlockCount_(0),
	stringStorageBlockMaxCount_(8),
	idStringTableSlots_(256),	///< Note - with this implementation of a hash table, using a prime number of slots gives no benefit over power-of-two number of slots
	idStringTableItemCount_(0)
	{
	// Allocate the array which stores the string block info
	stringStorageBlocks_=static_cast<StringStorageBlock*>(Malloc(stringStorageBlockMaxCount_*sizeof(StringStorageBlock)));

	// Allocate the first string block
	stringStorageBlocks_[0].head=static_cast<char*>(Malloc(stringStorageBlockSize_));
	stringStorageBlocks_[0].tail=stringStorageBlocks_[0].head;
	stringStorageBlockCount_++; 

	// Allocate the hash table
	idStringTable_=static_cast<char**>(Malloc(sizeof(char*)*idStringTableSlots_));

	// Mark all the slots of the string table as unused
	for (int i=0; i<idStringTableSlots_; i++)
		{
		idStringTable_[i]=0;
		}

	}

//*** Destructor ***

StringIdTable::~StringIdTable()
	{
	// Free all the string storage blocks
	for (int i=0; i<stringStorageBlockCount_; i++)
		{
		Free(stringStorageBlocks_[i].head);
		}
	
	// Free the array holding the string storage block info
	Free(stringStorageBlocks_);

	Free(idStringTable_);
	}


//*** CalculateHash ***

unsigned int StringIdTable::CalculateHash(const char* idString, int* stringLength) const
	{
	unsigned long hash = 5381; // Seed value

	// Modify hash for each character in the string
	const char* stringData=idString;
	while ( char c = *stringData )
		{
		// Convert character to upper case
		char u = (c <= 'z' && c >= 'a') ? c-('a'-'A') : c; 

		// A little bit-manipulation magic to get a nice distribution of values
        hash = ((hash << 5) + hash) ^ u;

		stringData++;
		}

	(*stringLength) = (stringData - idString);

	// Return the final hash value
	return hash;
 	}


//*** FindIdString ***

const char* StringIdTable::FindIdString(const char* idString)
	{
	// Calculate hash value for the string (and get the string length at the same time)
	int stringLength = 0;
	unsigned int hash = CalculateHash(idString, &stringLength);

	// Find slot for this string. This simply uses the passed in hash value, and modulates it by the number of slots. 
	// Note that as the number of slots is always a power-of-two-number, we can use a binary AND instead of modulo
	unsigned int slot=hash&(idStringTableSlots_-1);

	// Loop through all the entries until we find the one we are looking for or an empty slot
	char* entry=idStringTable_[slot];
	while(entry)
		{  
		// Is this the one we're looking for?
		unsigned int strhash=(*(reinterpret_cast<unsigned int*>(entry)));	// Hash number is the first four bytes
		if (strhash==hash && StrICmp(entry+4,idString)==0)
			{
			// Yes, so we just return the shared idString
			return entry+4; // Skip the first four bytes, as that's the hash number
			}
		
		// Not found the one we're looking for, so continue with the next entry 
		slot=(slot+1)&(idStringTableSlots_-1);
		entry=idStringTable_[slot];
		}

	// We didn't find an entry for that string, so we need to add it

	// If the table is more than two-thirds full, double its size and reinsert the strings
	if (idStringTableItemCount_>=(idStringTableSlots_-(idStringTableSlots_/3)))
		{
		// Make the new table twice the size
		int newidStringTableSlots=idStringTableSlots_*2;

		// Allocate memory for it
		char** newIdStringTable=static_cast<char**>(Malloc(sizeof(char*)*newidStringTableSlots));
		
		// And initialize all slots to unused
		for (int i=0; i<newidStringTableSlots; i++)
			{
			newIdStringTable[i]=0;
			}

		// Reinsert all the existing strings into the new table
		for (int i=0; i<idStringTableSlots_; i++)
			{
			// If slot is in use
			if (idStringTable_[i])
				{				
				// Get hash for string (stored as first four bytes of the string)
				unsigned int existinghash=(*(reinterpret_cast<unsigned int*>(idStringTable_[i])));

				// Calculate the slot
				int newslot=existinghash&(newidStringTableSlots-1);

				// Find the nearest unused slot
				while (newIdStringTable[newslot])
					{
					newslot=(newslot+1)&(newidStringTableSlots-1);
					}

				// Store the string in the new table
				newIdStringTable[newslot]=idStringTable_[i];
				}
			}
		
		// Free memory used by the old table
		Free(idStringTable_);

		// And replace the old table with the new one
		idStringTableSlots_=newidStringTableSlots;
		idStringTable_=newIdStringTable;

		// Calculate the new slot for the string we're currently inserting
		slot=hash&(idStringTableSlots_-1);

		// And find the nearest empty slot
		while (idStringTable_[slot])
			{
			slot=(slot+1)&(idStringTableSlots_-1);
			}
		}

	// Create a duplicate of the string
	char* newEntry=StoreString(hash, idString, stringLength); 
	
	// And store it in the table
	idStringTable_[slot]=newEntry;

	// Increase the total number of items stored
	idStringTableItemCount_++;

	// Return the shared id string
	return newEntry+4;	// Skip the first four bytes, as that's the hash number
	}


//*** StoreString ***
 
char* StringIdTable::StoreString(unsigned int hash, const char* string, int length)
	{
	// Calculate space needed to store the string
	int spaceNeeded=length+1+4; // Add space for terminator (1) and for storing hash number (4)

	// Check if there's space in the current string storage block
	StringStorageBlock* block=&stringStorageBlocks_[stringStorageBlockCount_-1]; // Current block is always the last one
	if ((stringStorageBlockSize_-(block->tail-block->head))<spaceNeeded)
		{
		// No more room - make a new storage block
		if (stringStorageBlockCount_==stringStorageBlockMaxCount_)
			{
			// Need a bigger array for string storage blocks
			stringStorageBlockMaxCount_*=2; // Go with twice the current size
			
			// Use realloc to get a bigger array while preserving the values it already holds
			stringStorageBlocks_=static_cast<StringStorageBlock*>(Realloc(stringStorageBlocks_,stringStorageBlockMaxCount_*sizeof(StringStorageBlock)));
			}

		// Allocate the new string storage block
		stringStorageBlocks_[stringStorageBlockCount_].head=static_cast<char*>(Malloc(stringStorageBlockSize_));
		stringStorageBlocks_[stringStorageBlockCount_].tail=stringStorageBlocks_[stringStorageBlockCount_].head; // Empty, so first free byte is at start

		// Update the current string storage block
		block=&stringStorageBlocks_[stringStorageBlockCount_];
		stringStorageBlockCount_++;
		}

	// Get the next piece of free memory from the current block
	char* strdest=block->tail;

	// Write hash number
	*(reinterpret_cast<unsigned int*>(strdest))=hash;

	// Copy the string to the block
	StrCpy(strdest+4,string); // Offset by four to account for hash number

	// Mark space as used
	block->tail+=spaceNeeded; 

	// Return pointer to the copy
	return strdest;
	}


