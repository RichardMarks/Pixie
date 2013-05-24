//*** PriorityQueueIterator.inl ***

#include "Debug.h"


//*** Constructor ***

template <class TYPE>
PriorityQueueIterator<TYPE>::PriorityQueueIterator(const PriorityQueue<TYPE>& queue):
	queue_(&queue),
	get_(0)
	{
	MoveFirst();	// Make sure it is reset to the first item on initialization
	}


//*** MoveFirst ***

template <class TYPE>
void PriorityQueueIterator<TYPE>::MoveFirst()
	{
	get_=0;
	}


//*** MoveNext***

template <class TYPE>
void PriorityQueueIterator<TYPE>::MoveNext()
	{
	if (get_<queue_.GetItemCount())
		{
		get_++;
		}
	}


//*** MoveLast ***

template <class TYPE>
void PriorityQueueIterator<TYPE>::MoveLast()
	{
	get_=queue_->GetItemCount()-1;
	}


//*** GetPrevious***

template <class TYPE>
void PriorityQueueIterator<TYPE>::MovePrevious()
	{
	if (get_>=0)
		{
		get_--;
		}
	}


//*** GetCurrent ***

template <class TYPE>
TYPE& PriorityQueueIterator<TYPE>::GetCurrent() const
	{
	Assert(get_>=0 && get_<queue_->GetItemCount(),"Invalid get location");
	if (get_>=0 && get_<queue_->GetItemCount())
		{
		return queue_->Get(get_);
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** IsValid ***

template <class TYPE>
bool PriorityQueueIterator<TYPE>::IsValid() const
	{
	if (get_>=0 && get_<queue_->GetItemCount())
		{
		return true;
		}

	return false;
	}


//*** Find ***

template <class TYPE>
bool PriorityQueueIterator<TYPE>::Find(const TYPE& data)
	{
	// Loop through all items
	for (int i=0; i<queue_->GetItemCount(); i++)
		{
		// Is the current item the one we are looking for?
		if (queue_->Get(i)==data)
			{
			// Yes, so set the get pointer to point at it
			get_=i;
			
			// And return true to indicate it was found
			return true;
			}
		}
	
	// The item was not found, so return false to indicate this
	return false;
	}


//*** GetCurrentIndex ***

template <class TYPE>
int PriorityQueueIterator<TYPE>::GetCurrentIndex() const
	{
	return get_;
	}
