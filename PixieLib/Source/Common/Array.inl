//*** Array.inl ***

#include "Debug.h"
#include "StandardLibrary.h"
#include <new>


//*** Constructor ***

template <class TYPE> 
Array<TYPE>::Array(int initialCapacity):
	initialCapacity_(initialCapacity),
	capacity_(initialCapacity),
	itemCount_(0),
	items_(0)
	{
	}	


//*** Copy Constructor ***

template <class TYPE> 
Array<TYPE>::Array(const Array<TYPE>& arrayToCopy ):
	initialCapacity_(arrayToCopy.initialCapacity_),
	capacity_(arrayToCopy.capacity_),
	itemCount_(arrayToCopy.itemCount_),
	items_(0)
	{
	if (!arrayToCopy.items_)
		{
		return;
		}

	// Allocate raw memory for entries
	items_=(TYPE*)Malloc(sizeof(TYPE)*capacity_);
	Assert(items_,"Couldn't allocate memory for array");
	if (!items_)
		{
		FatalError("Allocation failed when allocating memory for an array");
		}

	// Copy each item of the other array
	for (int i=0; i<itemCount_; i++)
		{
		// Copy using placement new and copy constructor
		new (&items_[i]) TYPE( arrayToCopy.items_[i] );
		}
	}	


//*** Assignment operator ***


template <class TYPE> 
const Array<TYPE>& Array<TYPE>::operator =(const Array<TYPE>& arrayToCopy)
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

	initialCapacity_=arrayToCopy.initialCapacity_;
	capacity_=arrayToCopy.capacity_;
	itemCount_=arrayToCopy.itemCount_;

	if (arrayToCopy.items_)
		{
		// Allocate raw memory for entries
		items_=(TYPE*)Malloc(sizeof(TYPE)*capacity_);
		Assert(items_,"Couldn't allocate memory for array");
		if (!items_)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item of the other array
		for (int i=0; i<itemCount_; i++)
			{
			// Copy using placement new and copy constructor
			new (&items_[i]) TYPE( arrayToCopy.items_[i] );
			}
		}

	return *this;
	}


//*** Destructor ***

template <class TYPE> 
Array<TYPE>::~Array()
	{
	// Call destructor manually on items
	for (int i=0; i<itemCount_; i++)
		{
		items_[i].~TYPE();
		}

	// Free memory used by the array
	if (items_)
		{
		// Release memory
		Free(items_);
		items_=0;
		}
	}


//*** Add ***

template <class TYPE> 
TYPE& Array<TYPE>::Add(const TYPE& item)
	{
	// Check if the array is full
	if (!items_ || itemCount_>=capacity_)
		{	
		int newCapacity=capacity_;
		if (itemCount_>=capacity_)
			{
			newCapacity*=2;
			}
		// Create a new array with twice the size
		// Allocate raw memory for entries
		TYPE* newItems=(TYPE*)Malloc(sizeof(TYPE)*newCapacity);
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
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

		// Delete the old array
		if (items_)
			{
			// Release memory
			Free(items_);
			}

		// Store the pointer to the new array instead of the old
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


//*** InsertBefore (index) ***

template <class TYPE> 
TYPE& Array<TYPE>::InsertBefore(int index, const TYPE& item)
	{
	Assert(index>=0 && index<itemCount_,"Index out of range");
	if (index<0 || index>=itemCount_)
		{
		static TYPE defaultValue;
		return defaultValue;
		}

	// Make room
	TYPE& result = Add(item);

	// If there were already items in the array, they need to be shifted
	if (itemCount_>1)
		{
		// Shift existing items 
		for (int i=itemCount_-1; i>index; i--)
			{
			items_[i].~TYPE();
			new (&items_[i]) TYPE(items_[i-1]);
			}
		
		// Insert the new item at the specified index
		items_[index].~TYPE();
		new (&items_[index]) TYPE(item);
		return items_[index];
		}

	return result;
	}


//*** InsertBefore (iterator) ***

template <class TYPE> 
TYPE& Array<TYPE>::InsertBefore(const ArrayIterator<TYPE>& insertBefore, const TYPE& item)
	{
	// Find index
	int index=insertBefore.GetCurrentIndex();
	Assert(index>=0 && index<itemCount_,"Iterator out of bounds");
	if (index>=0 && index<itemCount_)
		{
		// Insert the item
		return InsertBefore(index,item);
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** InsertAfter (index) ***

template <class TYPE> 
TYPE& Array<TYPE>::InsertAfter(int index, const TYPE& item)
	{
	Assert(index>=0 && index<itemCount_,"Index out of range");
	if (index<0 || index>=itemCount_)
		{
		static TYPE defaultValue;
		return defaultValue;
		}

	// Make room
	TYPE& result = Add(item);

	// We're actually interested in the item AFTER the specified index
	index++;

	// If there were already items in the array, they need to be shifted
	if (itemCount_>1)
		{
		// Shift existing items 
		for (int i=itemCount_-1; i>index; i--)
			{
			items_[i].~TYPE();
			new (&items_[i]) TYPE(items_[i-1]);
			}

		// Insert the new item at the specified index
		items_[index].~TYPE();
		new (&items_[index]) TYPE(item);
		result = items_[index];
		}
	
	return result;
	}


//*** InsertAfter (iterator) ***

template <class TYPE> 
TYPE& Array<TYPE>::InsertAfter(const ArrayIterator<TYPE>& insertAfter, const TYPE& item)
	{
	int index=insertAfter.GetCurrentIndex();
	Assert(index>=0 && index<itemCount_,"Iterator out of bounds");
	if (index>=0 && index<itemCount_)
		{
		// Insert the item
		return InsertAfter(index,item);
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** RemoveLast ***

template <class TYPE> 
void Array<TYPE>::RemoveLast()
	{
	Assert(items_ && itemCount_>0,"No items to remove");
	if (!items_ || itemCount_==0)
		{
		return;
		}
	
	// Do not downsize array - performance is more important than memory
	// and chances are that we will soon be adding a new item anyway
	itemCount_--;

	// Call destructor on removed object
	items_[itemCount_].~TYPE();	
	}


//*** Remove (index) ***

template <class TYPE> 
void Array<TYPE>::Remove(int index)
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
	
	// Move items after the item to be removed
	for (int i=index; i<itemCount_-1; i++)
		{
		items_[i].~TYPE();
		new (&items_[i]) TYPE(items_[i+1]);
		}
	
	// Decrease the total number of items
	itemCount_--;

	// Call destructor on removed object
	items_[itemCount_].~TYPE();	
	}
	

//*** Remove (iterator) ***

template <class TYPE> 
void Array<TYPE>::Remove(const ArrayIterator<TYPE>& iterator)
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
int Array<TYPE>::GetItemCount() const
	{
	return itemCount_;
	}


//*** Get (index) ***

template <class TYPE> 
TYPE& Array<TYPE>::Get(int index) const
	{
	Assert(items_ && index>=0 && index<itemCount_,"Index out of range");
	if (items_ && index>=0 && index<itemCount_)
		{
		return items_[index];
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** Get (iterator) ***

template <class TYPE> 
TYPE& Array<TYPE>::Get(const ArrayIterator<TYPE>& iterator) const
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


//*** Set (index) ***

template <class TYPE> 
void Array<TYPE>::Set(int index, const TYPE& item)
	{	
	Assert(index>=0,"Index out of range");
	if (index<0)
		{
		return;
		}

	// Check if we are indexing outside of the currently allocated range
	if (!items_ || index>=capacity_)
		{
		// Double the storage space
		int newCapacity=capacity_;

		// Keep doubling until we have enough
		while (newCapacity<index)
			{
			newCapacity*=2;
			}

		// Create a new array with the new size
		// Allocate raw memory for entries
		TYPE* newItems=(TYPE*)Malloc(sizeof(TYPE)*newCapacity);
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
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

		// Delete the old array
		if (items_)
			{
			// Release memory
			Free(items_);
			}

		// Store pointer to the new array
		items_=newItems;

		// Store the new value for maximum number of items
		capacity_=newCapacity;
		}

	// If we set an item outside of the currently used range, change the 
	// currently used range to reflect this
	if (index>=itemCount_)
		{
		// Placement new on items which are now valid part of array
		for (int i=itemCount_; i<=index; i++)
			{
			new (&items_[i]) TYPE;
			}

		itemCount_=index+1;
		}

	Assert(items_ && index>=0 && index<capacity_,"Index out of range");
	
	// Set the item
	if (items_ && index>=0 && index<capacity_)
		{
		items_[index].~TYPE();
		new (&items_[index]) TYPE(item);
		}
	}


//*** Set (iterator) ***

template <class TYPE> 
void Array<TYPE>::Set(const ArrayIterator<TYPE>& iterator, const TYPE& item)
	{	
	int index=iterator.GetCurrentIndex();
	Assert(index>=0,"Iterator out of bounds");
	if (index>=0)
		{
		Set(index,item);
		}
	}


//*** GetCapacity ***

template <class TYPE> 
int Array<TYPE>::GetCapacity() const
	{
	return capacity_;
	}


//*** SetCapacity ***

template <class TYPE> 
void Array<TYPE>::SetCapacity(int capacity)
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
		// Create a new array with the new size
		// Allocate raw memory for entries
		TYPE* newItems=(TYPE*)Malloc(sizeof(TYPE)*capacity);
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
		for (int i=0; i<itemCount_; i++)
			{
			// Copy using placement new and copy constructor
			new (&newItems[i]) TYPE( items_[i] );
			}

		// Delete the old array
		// Call destructor manually on existing items
		for (int i=0; i<itemCount_; i++)
			{
			items_[i].~TYPE();
			}

		// Release memory
		Free(items_);

		// Store pointer to the new array
		items_=newItems;
		}


	// Store the new value for maximum number of items
	capacity_=capacity;
	}


//*** Clear ***

template <class TYPE> 
void Array<TYPE>::Clear(bool releaseMemory)
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
bool Array<TYPE>::ItemExists(const TYPE& item) const
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
TYPE* Array<TYPE>::GetPointer() const
	{
	// Return the raw pointer to array memory, which might be dangerous, but that's the callers responsibility
	return items_;
	}
