//*** Array.inl ***

#include "Debug.h"

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

	items_=new TYPE[capacity_];

	Assert(items_,"Couldn't allocate memory for array");
	if (!items_)
		{
		FatalError("Allocation failed when allocating memory for an array");
		}

	// Copy each item of the other array
	for (int i=0; i<itemCount_; i++)
		{
		items_[i]=arrayToCopy.items_[i];
		}
	}	


//*** Assignment operator ***


template <class TYPE> 
const Array<TYPE>& Array<TYPE>::operator =(const Array<TYPE>& arrayToCopy)
	{ 
	if (items_)
		{
		delete[] items_;
		items_=0;
		}

	initialCapacity_=arrayToCopy.initialCapacity_;
	capacity_=arrayToCopy.capacity_;
	itemCount_=arrayToCopy.itemCount_;

	if (arrayToCopy.items_)
		{
		items_=new TYPE[capacity_];

		Assert(items_,"Couldn't allocate memory for array");
		if (!items_)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item of the other array
		for (int i=0; i<itemCount_; i++)
			{
			items_[i]=arrayToCopy.items_[i];
			}
		}

	return *this;
	}


//*** Destructor ***

template <class TYPE> 
Array<TYPE>::~Array()
	{
	// Free memory used by the array
	if (items_)
		{
		delete[] items_;
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
		TYPE* newItems=new TYPE[newCapacity];
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
		for (int i=0; i<itemCount_; i++)
			{
			newItems[i]=items_[i];
			}

		// Delete the old array
		if (items_)
			{
			delete[] items_;
			items_=0;
			}

		// Store the pointer to the new array instead of the old
		items_=newItems;

		// Increase the maximum number of items
		capacity_=newCapacity;
		}

	// Set the item
	items_[itemCount_]=item;
	TYPE& returnValue=items_[itemCount_];
	itemCount_++;
	return returnValue;
	}


//*** InsertBefore (index) ***

template <class TYPE> 
void Array<TYPE>::InsertBefore(const TYPE& item,int index)
	{
	Assert(index>=0 && index<itemCount_,"Index out of range");
	if (index<0 || index>=itemCount_)
		{
		return;
		}

	// Make room
	Add(item);

	// If there were already items in the array, they need to be shifted
	if (itemCount_>1)
		{
		// Shift existing items 
		for (int i=itemCount_-1; i>index; i--)
			{
			items_[i]=items_[i-1];
			}
		
		// Insert the new item at the specified index
		items_[index]=item;
		}
	}


//*** InsertBefore (iterator) ***

template <class TYPE> 
void Array<TYPE>::InsertBefore(const TYPE& item, const ArrayIterator<TYPE>& insertBefore)
	{
	// Find index
	int index=insertBefore.GetCurrentIndex();
	Assert(index>=0 && index<itemCount_,"Iterator out of bounds");
	if (index>=0 && index<itemCount_)
		{
		// Insert the item
		InsertBefore(item,index);
		}
	}


//*** InsertAfter (index) ***

template <class TYPE> 
void Array<TYPE>::InsertAfter(const TYPE& item,int index)
	{
	Assert(index>=0 && index<itemCount_,"Index out of range");
	if (index<0 || index>=itemCount_)
		{
		return;
		}

	// Make room
	Add(item);

	// We're actually interested in the item AFTER the specified index
	index++;

	// If there were already items in the array, they need to be shifted
	if (itemCount_>1)
		{
		// Shift existing items 
		for (int i=itemCount_-1; i>index; i--)
			{
			items_[i]=items_[i-1];
			}

		// Insert the new item at the specified index
		items_[index]=item;
		}
	}


//*** InsertAfter (iterator) ***

template <class TYPE> 
void Array<TYPE>::InsertAfter(const TYPE& item, const ArrayIterator<TYPE>& insertAfter)
	{
	int index=insertAfter.GetCurrentIndex();
	Assert(index>=0 && index<itemCount_,"Iterator out of bounds");
	if (index>=0 && index<itemCount_)
		{
		// Insert the item
		InsertAfter(item,index);
		}
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
		return;
		}
	
	// Move items after the item to be removed
	for (int i=index; i<itemCount_-1; i++)
		{
		items_[i]=items_[i+1];
		}
	
	// Decrease the total number of items
	itemCount_--;
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
	if (!items_ || index<0 || index>=itemCount_)
		{
		static TYPE defaultValue;
		return defaultValue;
		}

	return items_[index];
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
		TYPE* newItems=new TYPE[newCapacity];
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
		for (int i=0; i<itemCount_; i++)
			{
			newItems[i]=items_[i];
			}

		// Delete the old array
		if (items_)
			{
			delete[] items_;
			items_=0;
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
		itemCount_=index+1;
		}

	Assert(items_ && index>=0 && index<capacity_,"Index out of range");
	
	// Set the item
	if (items_ && index>=0 && index<capacity_)
		{
		items_[index]=item;
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

	if (itemCount_>capacity)
		{
		itemCount_=capacity;
		}

	if (items_)
		{
		// Create a new array with the new size
		TYPE* newItems=new TYPE[capacity];
		Assert(newItems,"Couldn't allocate memory for array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for array");
			}

		// Copy each item from the old array to the new one
		for (int i=0; i<itemCount_; i++)
			{
			newItems[i]=items_[i];
			}

		// Delete the old array
		delete[] items_;

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
	// Clear used range
	itemCount_=0;
	
	// Free allocated memory if requested
	if (releaseMemory)
		{
		// Free the memory
		if (items_)
			{
			delete[] items_;
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
