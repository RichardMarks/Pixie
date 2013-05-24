//*** MemoryPool.inl ***

#include "StandardLibrary.h"
#include "Debug.h"

#include <new>

//*** Constructor ***

template<class TYPE> 
MemoryPool<TYPE>::MemoryPool(int entriesPerBlock):
	entriesPerBlock_(entriesPerBlock),
	usedEntriesCount_(0),
	entries_(0),
	entriesSize_(0),
	entriesCount_(0),
	blocks_(0),
	blocksSize_(0),
	blocksCount_(0)
	{
	}


//*** Destructor ***

template<class TYPE> 
MemoryPool<TYPE>::~MemoryPool()
	{
	if (entries_)
		{
		delete[] entries_;
		}

	if (blocks_)
		{
		// Loop through and free the blocks.
		for (int i=0; i<blocksCount_; i++)
			{
			::Free(blocks_[i]);
			}

		::Free(blocks_);
		}
	}


//*** CreateBlock ***

template<class TYPE> 
void MemoryPool<TYPE>::CreateBlock()
	{
	// If no block array have been allocated yet, allocate it now
	if (!blocks_)
		{
		blocksSize_=16;
		blocks_=static_cast<TYPE**>(Malloc(sizeof(TYPE*)*blocksSize_));
		Assert(blocks_,"Couldn't allocate a memory block for the MemoryPool");
		if (!blocks_)
			{
			FatalError("Allocation failed when allocating memory for memory pool");
			}
		}

	// If the blocks array is full, double it and reallocate it
	if (blocksCount_>=blocksSize_)
		{
		blocksSize_*=2;
		blocks_=static_cast<TYPE**>(Realloc(blocks_,sizeof(TYPE*)*blocksSize_));
		Assert(blocks_,"Couldn't allocate a memory block for the MemoryPool");
		if (!blocks_)
			{
			FatalError("Allocation failed when allocating memory for memory pool");
			}
		}

	// Allocate a block of memory. 
	TYPE* newBlock = static_cast<TYPE*>(Malloc(sizeof(TYPE)*entriesPerBlock_));
	Assert(newBlock,"Couldn't allocate a memory block for the MemoryPool");
	if (!newBlock)
		{
		FatalError("Allocation failed when allocating memory for memory pool");
		}

	// Store a pointer to the allocated block
	blocks_[blocksCount_]=newBlock;
	blocksCount_++;

	// If no entries array have been allocated yet, allocate it now
	if (!entries_)
		{
		entriesSize_=entriesPerBlock_;
		entries_=static_cast<TYPE**>(Malloc(sizeof(TYPE*)*entriesPerBlock_));
		Assert(entries_,"Couldn't allocate a memory block for the MemoryPool");
		if (!entries_)
			{
			FatalError("Allocation failed when allocating memory for memory pool");
			}
		}

	// If the entries array is full, double it and reallocate it
	if (entriesCount_+entriesPerBlock_>entriesSize_)
		{
		entriesSize_*=2;
		entries_=static_cast<TYPE**>(Realloc(entries_,sizeof(TYPE*)*entriesSize_));
		Assert(entries_,"Couldn't allocate a memory block for the MemoryPool");
		if (!entries_)
			{
			FatalError("Allocation failed when allocating memory for memory pool");
			}
		}


	// Add all entries of the block to the list of unused entries
	for (int index = 0; index<entriesPerBlock_; index ++)
		{
		entries_[entriesCount_]=&newBlock[index];
		entriesCount_++;
		}
	}


//*** Allocate ***

template<class TYPE> 
TYPE* MemoryPool<TYPE>::Allocate()
	{
	// Check if there are any unused entries left
	if (entriesCount_== 0)
		{
		// If not, create a new block
		CreateBlock();
		}

	// Increase the number of currently used entries
	usedEntriesCount_++;

	// Remove the last of the unused entries from the list of unused entries 
	entriesCount_--;
	TYPE* memory=entries_[entriesCount_];

	// Create an object at the allocated memory position, and return it to the caller
	TYPE* object=new (memory) TYPE;
	return object;
	}


//*** Free ***

template<class TYPE> 
void MemoryPool<TYPE>::Free(TYPE* object)
	{
	// Call the objects destructor
	object->~TYPE();

	// Decrease the number of currently used entries
	usedEntriesCount_--;
	
	// If the entries array is full, double it and reallocate it
	if (entriesCount_>=entriesSize_)
		{
		entriesSize_*=2;
		entries_=static_cast<TYPE**>(Realloc(entries_,sizeof(TYPE*)*entriesSize_));
		Assert(entries_,"Couldn't allocate a memory block for the MemoryPool");
		if (!entries_)
			{
			FatalError("Allocation failed when allocating memory for memory pool");
			}
		}

	// Add entry to the list of unused entries
	entries_[entriesCount_]=object;
	entriesCount_++;
	}


//*** IsInUse ***

template<class TYPE> 
bool MemoryPool<TYPE>::IsInUse() const
	{
	return (usedEntriesCount_>0);
	}
