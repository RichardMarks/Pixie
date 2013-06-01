//*** PriorityQueueIterator.inl ***

#include "Debug.h"


//*** Constructor ***

template <typename TYPE, class COMPARE>
PriorityQueueIterator<TYPE, COMPARE>::PriorityQueueIterator(const PriorityQueue<TYPE, COMPARE>& queue):
	queue_(&queue),
	get_(0)
	{
	MoveFirst();	// Make sure it is reset to the first item on initialization
	}


//*** MoveFirst ***

template <typename TYPE, class COMPARE>
void PriorityQueueIterator<TYPE, COMPARE>::MoveFirst()
	{
	get_=0;
	}


//*** MoveNext***

template <typename TYPE, class COMPARE>
void PriorityQueueIterator<TYPE, COMPARE>::MoveNext()
	{
	if (get_<queue_.GetItemCount())
		{
		get_++;
		}
	}


//*** MoveLast ***

template <typename TYPE, class COMPARE>
void PriorityQueueIterator<TYPE, COMPARE>::MoveLast()
	{
	get_=queue_->GetItemCount()-1;
	}


//*** GetPrevious***

template <typename TYPE, class COMPARE>
void PriorityQueueIterator<TYPE, COMPARE>::MovePrevious()
	{
	if (get_>=0)
		{
		get_--;
		}
	}


//*** GetCurrent ***

template <typename TYPE, class COMPARE>
TYPE& PriorityQueueIterator<TYPE, COMPARE>::GetCurrent() const
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

template <typename TYPE, class COMPARE>
bool PriorityQueueIterator<TYPE, COMPARE>::IsValid() const
	{
	if (get_>=0 && get_<queue_->GetItemCount())
		{
		return true;
		}

	return false;
	}


//*** Find ***

template <typename TYPE, class COMPARE>
bool PriorityQueueIterator<TYPE, COMPARE>::Find(const TYPE& data)
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

template <typename TYPE, class COMPARE>
int PriorityQueueIterator<TYPE, COMPARE>::GetCurrentIndex() const
	{
	return get_;
	}
