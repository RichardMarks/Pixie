//*** BagIterator.inl ***

#include "Debug.h"


//*** Constructor ***

template <class TYPE>
BagIterator<TYPE>::BagIterator(const Bag<TYPE>& bag):
	bag_(&bag),
	get_(0)
	{
	MoveFirst();	// Make sure it is reset to the first item on initialization
	}


//*** MoveFirst ***

template <class TYPE>
void BagIterator<TYPE>::MoveFirst()
	{
	get_=0;
	}


//*** MoveNext***

template <class TYPE>
void BagIterator<TYPE>::MoveNext()
	{
	if (get_<bag_->GetItemCount())
		{
		get_++;
		}
	}


//*** MovePrevious***

template <class TYPE>
void BagIterator<TYPE>::MovePrevious()
	{
	if (get_>=0)
		{
		get_--;
		}
	}


//*** MoveLast ***

template <class TYPE>
void BagIterator<TYPE>::MoveLast()
	{
	get_=bag_->GetItemCount()-1;
	}


//*** IsValid ***

template <class TYPE>
bool BagIterator<TYPE>::IsValid() const
	{
	if (get_>=0 && get_<bag_->GetItemCount())
		{
		return true;
		}

	return false;
	}


//*** GetCurrent ***

template <class TYPE>
TYPE& BagIterator<TYPE>::GetCurrent() const
	{
	Assert(get_>=0 && get_<bag_->GetItemCount(),"Invalid get location");
	if (get_>=0 && get_<bag_->GetItemCount())
		{
		return bag_->Get(get_);
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** GetCurrentIndex ***

template <class TYPE>
int BagIterator<TYPE>::GetCurrentIndex() const
	{
	return get_;
	}


//*** Find ***

template <class TYPE>
bool BagIterator<TYPE>::Find(const TYPE& data)
	{
	// Loop through all items
	for (int i=0; i<bag_->GetItemCount(); i++)
		{
		// Is the current item the one we are looking for?
		if (bag_->Get(i)==data)
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
