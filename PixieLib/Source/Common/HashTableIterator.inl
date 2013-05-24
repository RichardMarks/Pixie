//*** HashTableIterator.inl ***

#include "Debug.h"

//*** Constructor ***

template<class HASHTABLEKEY, class TYPE> 
HashTableIterator<HASHTABLEKEY, TYPE>::HashTableIterator(const HashTable<HASHTABLEKEY, TYPE>& table):
	table_(&table),
	currentGetIndex_(0)
	{
	MoveFirst();
	}


//*** MoveFirst ***

template<class HASHTABLEKEY, class TYPE> 
void HashTableIterator<HASHTABLEKEY, TYPE>::MoveFirst()
	{
	if (!table_->items_ || table_->GetItemCount()==0)
		{
		currentGetIndex_=-1;
		return;
		}

	// Set the current pointers to look at the first slot	
	currentGetIndex_=0;

	// Loop through all the slots until we find one that is in use
	while (!table_->items_[currentGetIndex_].inUse && currentGetIndex_<table_->slotCount_)
		{
		// Proceed to next slot
		currentGetIndex_++;
		}
	}


//*** MoveNext***

template<class HASHTABLEKEY, class TYPE> 
void HashTableIterator<HASHTABLEKEY, TYPE>::MoveNext()
	{
	if (!table_->items_ || table_->GetItemCount()==0)
		{
		currentGetIndex_=-1;
		return;
		}

	// Check if we are currently looking at a valid item
	if (!IsValid())
		{
		// If not, abort and return 0 (no next item found)
		return;
		}
	
	// Get the next item 
	currentGetIndex_++;

	// Loop through all the slots until we find one that is in use
	while (!table_->items_[currentGetIndex_].inUse && currentGetIndex_<table_->slotCount_)
		{
		// Proceed to next slot
		currentGetIndex_++;
		}
	}


//*** MoveLast ***

template<class HASHTABLEKEY, class TYPE> 
void HashTableIterator<HASHTABLEKEY, TYPE>::MoveLast()
	{
	if (!table_->items_ || table_->GetItemCount()==0)
		{
		currentGetIndex_=-1;
		return;
		}

	// Set the current index pointers to look at the last slot	
	currentGetIndex_=table_->slotCount_-1;

	// Loop through all the slots until we find one that is in used
	while (!table_->items_[currentGetIndex_].inUse && currentGetIndex_>=0)
		{
		// Proceed to previous slot
		currentGetIndex_--;
		}
	}


//*** MovePrevious ***

template<class HASHTABLEKEY, class TYPE> 
void HashTableIterator<HASHTABLEKEY, TYPE>::MovePrevious()
	{
	// Check if we are currently looking at a valid item
	if (!IsValid())
		{
		// If not, abort and return 0 (no next item found)
		return;
		}
	
	// Get the previous item 
	currentGetIndex_--;

	// Loop through all the slots until we find one that is in used
	while (!table_->items_[currentGetIndex_].inUse && currentGetIndex_>=0)
		{
		// Proceed to previous slot
		currentGetIndex_--;
		}
	}


//*** GetCurrent ***

template<class HASHTABLEKEY, class TYPE> 
TYPE& HashTableIterator<HASHTABLEKEY, TYPE>::GetCurrent() const
	{
	Assert(IsValid(),"Invalid get location");

	// Check if we are currently looking at a valid item
	if (!IsValid())
		{
		// If not, abort and return 0 (no current item found)
		static TYPE defaultValue;
		return defaultValue;
		}

	// Return the item
	return table_->items_[currentGetIndex_].data;
	}


//*** IsValid ***

template<class HASHTABLEKEY, class TYPE> 
bool HashTableIterator<HASHTABLEKEY, TYPE>::IsValid() const
	{
	if (!table_->items_ || table_->GetItemCount()==0)
		{
		return false;
		}

	if (currentGetIndex_<0 || currentGetIndex_>=table_->slotCount_)
		{
		return false;
		}
	
	return table_->items_[currentGetIndex_].inUse;
	}


//*** Find ***

template<class HASHTABLEKEY, class TYPE> 
bool HashTableIterator<HASHTABLEKEY, TYPE>::Find(const HASHTABLEKEY& key)
	{	
	if (!table_->items_ || table_->GetItemCount()==0)
		{
		currentGetIndex_=-1;
		return false;
		}

	// Find the slot for this key		
	int slot = key.GetHash() & (table_->slotCount_-1);
	int assignedSlot=slot;
	int itemsAssignedToSlot=table_->items_[slot].itemCount;
	
	// Loop through the items until we find the one we are looking for
	while (itemsAssignedToSlot>0)
		{

		if (table_->items_[slot].inUse)
			{
			int thisSlot=table_->items_[slot].key.GetHash()&(table_->slotCount_-1);
			if (thisSlot==assignedSlot)
				{
				itemsAssignedToSlot--;

				// Is this the one we're looking for?
				if (key.GetHash()==table_->items_[slot].key.GetHash() && key.Compare(&table_->items_[slot].key))
					{
					// Yes, so set the current get pointers to point at it
					currentGetIndex_=slot;
					
					// Item found
					return true;
					}
				}
			}
		
		// Not found the one we're looking for, so continue with the next item
		slot=(slot+1)&(table_->slotCount_-1);
		}

	// We didn't find the item
	return false;
	}


//*** GetCurrentKey ***

template<class HASHTABLEKEY, class TYPE> 
const HASHTABLEKEY& HashTableIterator<HASHTABLEKEY, TYPE>::GetCurrentKey() const
	{
	// Check if we are looking at a valid item
	if (!IsValid())
		{
		static HASHTABLEKEY defaultValue;
		return defaultValue;
		}
		
	// Return the key for the current get item
	return table_->items_[currentGetIndex_].key;
	}

