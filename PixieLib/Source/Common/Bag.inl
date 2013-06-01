//*** Bag.inl ***

#include "Debug.h"
#include "StandardLibrary.h"
#include <new>

//*** Constructor ***

template <class TYPE> 
Bag<TYPE>::Bag(int initialCapacity):
	initialCapacity_(initialCapacity),
	capacity_(initialCapacity),
	itemCount_(0),
	items_(0)
	{
	}	


//*** Copy Constructor ***

template <class TYPE> 
Bag<TYPE>::Bag(const Bag<TYPE>& bagToCopy ):
	initialCapacity_(bagToCopy.initialCapacity_),
	capacity_(bagToCopy.capacity_),
	itemCount_(bagToCopy.itemCount_),
	items_(0)
	{
	if (!bagToCopy.items_)
		{
		return;
		}

	// Allocate raw memory for entries
	items_=(TYPE*)Malloc(sizeof(TYPE)*capacity_);
	Assert(items_,"Couldn't allocate memory for bag");
	if (!items_)
		{
		FatalError("Allocation failed when allocating memory for a bag");
		}	

	// Copy each item of the other bag
	for (int i=0; i<itemCount_; i++)
		{
		// Copy using placement new and copy constructor
		new (&items_[i]) TYPE( bagToCopy.items_[i] );
		}
	}	


//*** Assignment operator ***

template <class TYPE> 
const Bag<TYPE>& Bag<TYPE>::operator =(const Bag<TYPE>& bagToCopy)
	{
	// Call destructor manually on existing items
	for (int i=0; i<itemCount_; i++)
		{
		items_[i].~TYPE();
		}

	if (items_)
		{
		// Release memory
		Free(items_);
		items_=0;
		}

	initialCapacity_=bagToCopy.initialCapacity_;
	capacity_=bagToCopy.capacity_;
	itemCount_=bagToCopy.itemCount_;

	if (bagToCopy.items_)
		{
		// Allocate raw memory for entries
		items_=(TYPE*)Malloc(sizeof(TYPE)*capacity_);
		Assert(items_,"Couldn't allocate memory for bag");
		if (!items_)
			{
			FatalError("Allocation failed when allocating memory for bag");
			}

		// Copy each item of the other bag
		for (int i=0; i<itemCount_; i++)
			{
			// Copy using placement new and copy constructor
			new (&items_[i]) TYPE( bagToCopy.items_[i] );
			}
		}

	return *this;
	}


//*** Destructor ***

template <class TYPE> 
Bag<TYPE>::~Bag()
	{
	// Call destructor manually on items
	for (int i=0; i<itemCount_; i++)
		{
		items_[i].~TYPE();
		}
	
	// Free memory used by the bag
	if (items_)
		{
		// Release memory
		Free(items_);
		items_=0;
		}
	}


//*** Add ***

template <class TYPE> 
TYPE& Bag<TYPE>::Add(const TYPE& item)
	{
	// Check if the bag is full
	if (!items_ || itemCount_>=capacity_)
		{	
		int newCapacity=capacity_;
		if (itemCount_>=capacity_)
			{
			newCapacity*=2;
			}

		// Create a new bag with twice the size
		// Allocate raw memory for entries
		TYPE* newItems=(TYPE*)Malloc(sizeof(TYPE)*newCapacity);
		Assert(newItems,"Couldn't allocate memory for bag");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for bag");
			}

		// Copy each item from the old bag to the new one
		for (int i=0; i<itemCount_; i++)
			{
			// Copy using placement new and copy constructor
			new (&newItems[i]) TYPE( items_[i] );
			}

		// Call destructor manually on items to be deleted
		for (int i=0; i<itemCount_; i++)
			{
			items_[i].~TYPE();
			}
		
		// Delete the old bag
		if (items_)
			{
			// Release memory
			Free(items_);
			}

		// Store the pointer to the new bag instead of the old
		items_=newItems;

		// Increase the maximum number of items
		capacity_=newCapacity;
		}

	// Set the item
	new (&items_[itemCount_]) TYPE(item); // Placement new to copy item

	TYPE& returnValue=items_[itemCount_];
	itemCount_++;

	return returnValue;
	}


//*** RemoveLast ***

template <class TYPE> 
void Bag<TYPE>::RemoveLast()
	{
	Assert(items_ && itemCount_>0,"No items to remove");
	if (!items_ || itemCount_==0)
		{
		return;
		}

	// Do not downsize bag - performance is more important than memory
	// and chances are that we will soon be adding a new item anyway
	itemCount_--;

	// Call destructor on removed object
	items_[itemCount_].~TYPE();	
	}


//*** Remove (index) ***

template <class TYPE> 
void Bag<TYPE>::Remove(int index)
	{
	Assert(items_ && index>=0 && index<itemCount_,"Index out of range");
	if (!items_ || index<0 || index>=itemCount_)
		{
		return;
		}

	// If the item to remove is the last item, just decrease the number of items and return
	if (index==itemCount_-1)
		{
		itemCount_--;

		// Call destructor on removed object
		items_[itemCount_].~TYPE();
		return;
		}

	// Decrease the total number of items
	itemCount_--;

	// Move the last item into the slot of the item we are removing
	items_[index].~TYPE();	
	new (&items_[index]) TYPE(items_[itemCount_]);
	
	// Call destructor on removed object
	items_[itemCount_].~TYPE();
	}
	

//*** Remove (iterator) ***

template <class TYPE> 
void Bag<TYPE>::Remove(const BagIterator<TYPE>& iterator)
	{
	int index=iterator.GetCurrentIndex();
	Assert(index>=0 && index<itemCount_,"Iterator out of bounds");
	if (index>=0 && index<itemCount_)
		{
		// Remove the item
		Remove(index);
		}
	}


//*** GetItemCount ***

template <class TYPE> 
int Bag<TYPE>::GetItemCount() const
	{
	return itemCount_;
	}


//*** Get (index) ***

template <class TYPE> 
TYPE& Bag<TYPE>::Get(int index) const
	{
	Assert(items_ && index>=0 && index<itemCount_,"Index out of range");
	if (!items_ || index<0 || index>=itemCount_)
		{
		static TYPE defaultValue;
		return defaultValue;
		}

	return items_[index];
	}


//*** Get (iterator) ***

template <class TYPE> 
TYPE& Bag<TYPE>::Get(const BagIterator<TYPE>& iterator) const
	{
	int index=iterator.GetCurrentIndex();
	Assert(items_ && index>=0 && index<itemCount_,"Iterator out of bounds");
	if (!items_ || index<0 || index>=itemCount_)
		{
		static TYPE defaultValue;
		return defaultValue;
		}

	return items_[index];
	}


//*** GetCapacity ***

template <class TYPE> 
int Bag<TYPE>::GetCapacity() const
	{
	return capacity_;
	}


//*** SetCapacity ***

template <class TYPE> 
void Bag<TYPE>::SetCapacity(int capacity)
	{
	Assert(capacity>0,"Invalid capacity");
	if (capacity<=0)
		{
		return;
		}

	// Truncate array if necessary
	if (itemCount_>capacity)
		{
		// Call destructor manually on extra items
		for (int i=capacity; i<itemCount_; i++)
			{
			items_[i].~TYPE();
			}

		itemCount_=capacity;
		}

	if (items_)
		{
		// Create a new bag with the new size
		// Allocate raw memory for entries
		TYPE* newItems=(TYPE*)Malloc(sizeof(TYPE)*capacity);
		Assert(newItems,"Couldn't allocate memory for bag");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for bag");
			}

		// Copy each item from the old bag to the new one
		for (int i=0; i<itemCount_; i++)
			{
			// Copy using placement new and copy constructor
			new (&newItems[i]) TYPE( items_[i] );
			}

		// Delete the old bag
		// Call destructor manually on existing items
		for (int i=0; i<itemCount_; i++)
			{
			items_[i].~TYPE();
			}
	
		// Release memory
		Free(items_);

		// Store pointer to the new bag
		items_=newItems;
		}


	// Store the new value for maximum number of items
	capacity_=capacity;
	}


//*** Clear ***

template <class TYPE> 
void Bag<TYPE>::Clear(bool releaseMemory)
	{	
	// Call destructor manually on existing items
	for (int i=0; i<itemCount_; i++)
		{
		items_[i].~TYPE();
		}

	// Clear used range
	itemCount_=0;

	// Free allocated memory if requested
	if (releaseMemory)
		{
		// Free the memory
		if (items_)
			{		
			// Release memory
			Free(items_);
			items_=0;
			}
		
		// Reset to initial size
		capacity_=initialCapacity_;

		}

	}


//*** ItemExists ***

template <class TYPE>
bool Bag<TYPE>::ItemExists(const TYPE& item) const
	{
	for (int i=0; i<itemCount_; i++)
		{
		if (items_[i]==item)
			{
			return true;
			}
		}

	return false;
	}



//*** GetPointer ***

template <class TYPE> 
TYPE* Bag<TYPE>::GetPointer() const
	{
	// Return the raw pointer to bag memory, which might be dangerous, but that's the callers responsibility
	return items_;
	}
