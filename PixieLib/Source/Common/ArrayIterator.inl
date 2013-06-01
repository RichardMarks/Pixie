//*** ArrayIterator.inl ***

#include "Debug.h"


//*** Constructor ***

template <class TYPE>
ArrayIterator<TYPE>::ArrayIterator(const Array<TYPE>& array):
	array_(&array),
	get_(0)
	{
	MoveFirst();	// Make sure it is reset to the first item on initialization
	}


//*** MoveFirst ***

template <class TYPE>
void ArrayIterator<TYPE>::MoveFirst()
	{
	get_=0;
	}


//*** MoveNext***

template <class TYPE>
void ArrayIterator<TYPE>::MoveNext()
	{
	if (get_<array_->GetItemCount())
		{
		get_++;
		}
	}


//*** MovePrevious***

template <class TYPE>
void ArrayIterator<TYPE>::MovePrevious()
	{
	if (get_>=0)
		{
		get_--;
		}
	}


//*** MoveLast ***

template <class TYPE>
void ArrayIterator<TYPE>::MoveLast()
	{
	get_=array_->GetItemCount()-1;
	}


//*** IsValid ***

template <class TYPE>
bool ArrayIterator<TYPE>::IsValid() const
	{
	if (get_>=0 && get_<array_->GetItemCount())
		{
		return true;
		}

	return false;
	}


//*** GetCurrent ***

template <class TYPE>
TYPE& ArrayIterator<TYPE>::GetCurrent() const
	{
	Assert(get_>=0 && get_<array_->GetItemCount(),"Invalid get location");
	if (get_>=0 && get_<array_->GetItemCount())
		{
		return array_->Get(get_);
		}

	static TYPE defaultValue;
	return defaultValue;
	}


//*** GetCurrentIndex ***

template <class TYPE>
int ArrayIterator<TYPE>::GetCurrentIndex() const
	{
	return get_;
	}


//*** Find ***

template <class TYPE>
bool ArrayIterator<TYPE>::Find(const TYPE& data)
	{
	// Loop through all items
	for (int i=0; i<array_->GetItemCount(); i++)
		{
		// Is the current item the one we are looking for?
		if (array_->Get(i)==data)
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
