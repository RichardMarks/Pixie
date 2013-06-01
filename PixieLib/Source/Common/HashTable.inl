//*** HashTable.cpp ***

#include "HashTable.h"
#include "Debug.h"

//*** Constructor ***

template<class HASHTABLEKEY, class TYPE> 
HashTable<HASHTABLEKEY, TYPE>::HashTable(int initialSlots):
	initialSlots_(initialSlots),
	slotCount_(0),
	itemCount_(0),
	items_(0)
	{
	Assert(initialSlots_>0,"Number of slots must be > 0");
	if (initialSlots_<=0)
		{
		initialSlots_=64;
		}

	// Make sure number of slots is power-of-two number
	int s=1;
	while (s<initialSlots_)
		{
		s*=2;
		}
	initialSlots_=s;

	slotCount_=initialSlots_;
	}


//*** Destructor ***

template<class HASHTABLEKEY, class TYPE> 
HashTable<HASHTABLEKEY, TYPE>::~HashTable()
	{
	// Delete the array that held all the items
	if (items_)
		{
		delete[] items_;
		}
	}


//*** Insert ***

template<class HASHTABLEKEY, class TYPE> 
TYPE& HashTable<HASHTABLEKEY, TYPE>::Insert(const HASHTABLEKEY& key, const TYPE& data)
	{
	// If the table is more than two-thirds full, double its size and reinsert the items
	if (itemCount_>=(slotCount_-(slotCount_/3)))
		{
		// Make the new table twice the size
		int newSlotCount=slotCount_*2;

		// Allocate memory for it
		HashTableItem* newTable=new HashTableItem[newSlotCount];
		
		// Reinsert all the existing items into the new table
		if (items_)
			{
			for (int i=0; i<slotCount_; i++)
				{
				// If slot is in use
				HashTableItem& item=items_[i];
				if (item.inUse)
					{				
					// Get hash for item
					unsigned int existinghash=item.key.GetHash();

					// Calculate the slot
					int newslot=existinghash&(newSlotCount-1);
					newTable[newslot].itemCount++;

					// Find the nearest unused slot
					while (newTable[newslot].inUse)
						{
						newslot=(newslot+1)&(newSlotCount-1);
						}

					// Store the item in the new table
					newTable[newslot].key=items_[i].key;
					newTable[newslot].data=items_[i].data;
					newTable[newslot].inUse=true;
					}
				}
			
			// Free memory used by the old table
			delete[] items_;
			}

		// And replace the old table with the new one
		slotCount_=newSlotCount;
		items_=newTable;
		}

	// Create the array that will hold all the items, if it hasn't been created yet
	if (!items_)
		{
		items_=new HashTableItem[slotCount_];
		}
	

	// Find slot for this item. This simply uses the hash value of the key, and modulates it by the number of slots. 
	// Note that as the number of slots is always a power-of-two-number, we can use a binary AND instead of modulo
	int slot=key.GetHash()&(slotCount_-1);

	// Keep track of how many items that have this slot as their assigned slot, even though they might be stored in 
	// a forward slot due to collisions
	items_[slot].itemCount++; 

	// Loop through all the entries until we find an empty slot
	while(items_[slot].inUse)
		{  
		// Entry not empty, so continue with the next entry 
		slot=(slot+1)&(slotCount_-1);
		}


	// Store item in the table
	HashTableItem& newItem=items_[slot];
	newItem.inUse=true;
	newItem.key=key;
	newItem.data=data;

	// Increase the number of items		
	itemCount_++;
	return newItem.data;
	}


//*** Remove ***

template<class HASHTABLEKEY, class TYPE> 
void HashTable<HASHTABLEKEY, TYPE>::Remove(const HashTableIterator<HASHTABLEKEY,TYPE>& iterator)
	{
	Assert(iterator.IsValid(),"Invalid iterator");
	if (!iterator.IsValid())
		{
		return;
		}

	int slot=iterator.GetCurrentKey().GetHash()&(slotCount_-1);
	items_[slot].itemCount--;
	items_[iterator.currentGetIndex_].inUse=false;

	// Decrease the number of items
	itemCount_--;
	}


//*** Remove ***

template<class HASHTABLEKEY, class TYPE> 
void HashTable<HASHTABLEKEY, TYPE>::Remove(const HASHTABLEKEY& key)
	{
	Assert(itemCount_>0 && items_,"Table is empty");
	if (itemCount_==0 || !items_)
		{
		return;
		}

	// Find slot for this item. This simply uses the hash value of the key, and modulates it by the number of slots. 
	// Note that as the number of slots is always a power-of-two-number, we can use a binary AND instead of modulo
	int slot=key.GetHash()&(slotCount_-1);
	int assignedSlot=slot;
	int itemsAssignedToSlot=items_[slot].itemCount;

	while (itemsAssignedToSlot>0)
		{

		if (items_[slot].inUse)
			{
			int thisSlot=items_[slot].key.GetHash()&(slotCount_-1);
			if (thisSlot==assignedSlot)
				{
				itemsAssignedToSlot--;

				// Is this the item we're looking for?
				if (items_[slot].key.GetHash()==key.GetHash() && items_[slot].key.Compare(&key))
					{
					// Mark item as unused
					items_[slot].inUse=false;


					// Decrease the number of items assigned to the slot
					items_[assignedSlot].itemCount--;

					// Decrease the number of items
					itemCount_--;
					return;
					}
				}
			}

		slot=(slot+1)&(slotCount_-1);
		}

	Assert(false,"Item not found");
	}


//*** GetItemCount ***

template<class HASHTABLEKEY, class TYPE> 
int HashTable<HASHTABLEKEY, TYPE>::GetItemCount() const
	{
	return itemCount_;
	}


//*** Clear ***

template<class HASHTABLEKEY, class TYPE> 
void HashTable<HASHTABLEKEY, TYPE>::Clear(bool releaseMemory)
	{
	// Delete the array and reset values
	if (releaseMemory && items_)
		{
		delete[] items_;
		items_=0;
		slotCount_=initialSlots_;
		itemCount_=0;
		return;
		}
	
	// Loop through all the slots and mark all as unused
	if (items_)
		{
		for(int i=0; i<slotCount_; i++)
			{
			items_[i].inUse=false;
			items_[i].itemCount=0;
			}
		}

	// Reset the item count	
	itemCount_=0;
	}

