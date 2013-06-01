//*** PriorityQueue.inl ***

#include "Debug.h"
#include "StandardLibrary.h"

//*** Constructor ***

template <typename TYPE, class COMPARE> 
PriorityQueue<TYPE, COMPARE>::PriorityQueue(int initialCapacity):
	initialCapacity_(initialCapacity),
	capacity_(initialCapacity),
	itemCount_(0),
	items_(0)	
	{
	}	


//*** Copy Constructor ***

template <typename TYPE, class COMPARE> 
PriorityQueue<TYPE, COMPARE>::PriorityQueue(const PriorityQueue<TYPE, COMPARE>& priorityQueueToCopy ):
	initialCapacity_(priorityQueueToCopy.initialCapacity_),
	capacity_(priorityQueueToCopy.capacity_),
	itemCount_(priorityQueueToCopy.itemCount_),
	items_(0)
	{
	if (!priorityQueueToCopy.items_)
		{
		return;
		}

	items_=new TYPE[capacity_];

	Assert(items_,"Couldn't allocate memory for priority queue");
	if (!items_)
		{
		FatalError("Allocation failed when allocating memory for priority queue");
		}

	// Copy each item of the other array
	for (int i=0; i<itemCount_; i++)
		{
		items_[i]=priorityQueueToCopy.items_[i];
		}
	}	

//*** Assignment operator ***


template <typename TYPE, class COMPARE> 
const PriorityQueue<TYPE, COMPARE>& PriorityQueue<TYPE, COMPARE>::operator =(const PriorityQueue<TYPE, COMPARE>& priorityQueueToCopy)
	{ 
	if (items_)
		{
		delete[] items_;
		items_=0;
		}

	initialCapacity_=priorityQueueToCopy.initialCapacity_;
	capacity_=priorityQueueToCopy.capacity_;
	itemCount_=priorityQueueToCopy.itemCount_;

	if (priorityQueueToCopy.items_)
		{
		items_=new TYPE[capacity_];

		Assert(items_,"Couldn't allocate memory for priority queue");
		if (!items_)
			{
			FatalError("Allocation failed when allocating memory for priority queue");
			}

		// Copy each item of the other array
		for (int i=0; i<itemCount_; i++)
			{
			items_[i]=priorityQueueToCopy.items_[i];
			}
		}

	return *this;
	}



//*** Destructor ***

template <typename TYPE, class COMPARE> 
PriorityQueue<TYPE, COMPARE>::~PriorityQueue()
	{
	// Free memory used by the array
	if (items_)
		{
		delete[] items_;
		items_=0;
		}
	}

//*** Add ***

template <typename TYPE, class COMPARE> 
TYPE& PriorityQueue<TYPE, COMPARE>::Add(const TYPE& item)
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
		Assert(newItems,"Couldn't allocate memory for dynamic array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for priority queue");
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
	itemCount_++;


	// "Bubble" the item to the right position in the tree
	int index=itemCount_;
	while (index>1 && COMPARE::Compare(items_[index-1],items_[index/2-1]))
		{
		// Swap items
		TYPE temp=items_[index/2-1];
		items_[index/2-1]=items_[index-1];
		items_[index-1]=temp;

		index=index/2;
		}

	return items_[index-1];
	}


//*** Remove ***

template <typename TYPE, class COMPARE> 
TYPE PriorityQueue<TYPE, COMPARE>::Remove()
	{
	if (itemCount_==0)
		{
		return TYPE();
		}

	TYPE returnValue=items_[0];
	items_[0]=items_[itemCount_-1];
	itemCount_--;

	if (itemCount_==0)
		{
		return returnValue;
		}

	int v=1;
	int u=0;
	while (u!=v)
		{
		u=v;
		// If both children exist
		if ((2*u+1)<=itemCount_) 
			{
			// Select the lowest of the two children.
			if (!COMPARE::Compare(items_[u-1],items_[2*u -1]))
				{
				v=2*u;
				}
			if (!COMPARE::Compare(items_[v-1],items_[2*u+1 -1]))
				{
				v=2*u+1;
				}
			}

		// If only child #1 exists
		else if (2*u<=itemCount_)
			{
			// Check if the cost is greater than the child
			if (!COMPARE::Compare(items_[u-1],items_[2*u-1]))
				{
				v=2*u;
				}
			}

		if (u!=v)
			{
			// Swap items
			TYPE temp=items_[u-1];
			items_[u-1]=items_[v-1];
			items_[v-1]=temp;
			}
		}

	return returnValue;
	}
	

//*** Update ***

template <typename TYPE, class COMPARE> 
void PriorityQueue<TYPE, COMPARE>::Update(int index)
	{
	while (index>1 && COMPARE::Compare(items_[index-1],items_[index/2-1]))
		{
		// Swap items
		TYPE temp=items_[index/2-1];
		items_[index/2-1]=items_[index-1];
		items_[index-1]=temp;

		index=index/2;
		}
	}


//*** Clear ***

template <typename TYPE, class COMPARE> 
void PriorityQueue<TYPE, COMPARE>::Clear(bool releaseMemory)
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

template <typename TYPE, class COMPARE>
bool PriorityQueue<TYPE, COMPARE>::ItemExists(const TYPE& item) const
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


//*** GetItemCount ***

template <typename TYPE, class COMPARE> 
int PriorityQueue<TYPE, COMPARE>::GetItemCount() const
	{
	return itemCount_;
	}


//*** Get ***

template <typename TYPE, class COMPARE> 
TYPE& PriorityQueue<TYPE, COMPARE>::Get(int index) const
	{
	return items_[index];
	}


//*** GetCapacity ***

template <typename TYPE, class COMPARE> 
int PriorityQueue<TYPE, COMPARE>::GetCapacity() const
	{
	return capacity_;
	}


//*** SetCapacity ***

template <typename TYPE, class COMPARE> 
void PriorityQueue<TYPE, COMPARE>::SetCapacity(int capacity)
	{
	if (itemCount_>capacity)
		{
		itemCount_=capacity;
		}

	if (items_)
		{
		// Create a new array with the new size
		TYPE* newItems=new TYPE[capacity];
		Assert(newItems,"Couldn't allocate memory for dynamic array");
		if (!newItems)
			{
			FatalError("Allocation failed when allocating memory for priority queue");
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
