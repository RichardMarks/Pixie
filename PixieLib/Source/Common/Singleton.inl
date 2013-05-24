//*** Singleton.inl ***

#include "Debug.h"

// Static instance pointer
template <class TYPE> TYPE* Singleton<TYPE>::staticInstancePointer_=0;


//*** Constructor ***

template <class TYPE> 
Singleton<TYPE>::Singleton()
	{
	if (staticInstancePointer_)
		{
		FatalError("Attempt to create multiple instances of a singleton system.");
		}

	staticInstancePointer_=static_cast<TYPE*>(this);
	}


//*** Destructor ***

template <class TYPE> 
Singleton<TYPE>::~Singleton()
	{
	if (!staticInstancePointer_)
		{
		FatalError("Attempt to destroy singleton instance when it has not been created.");
		}
	staticInstancePointer_=0;
	}


//*** CreateInstance ***

template <class TYPE> 
void Singleton<TYPE>::CreateInstance()
	{
	if (staticInstancePointer_)
		{
		FatalError("Attempt to create multiple instances of a singleton system.");		
		}
	
	staticInstancePointer_=new TYPE();
	}


//*** DestroyInstance ***

template <class TYPE> 
void Singleton<TYPE>::DestroyInstance()
	{
	if (!staticInstancePointer_)
		{
		FatalError("Attempt to destroy singleton instance when it has not been created.");
		}
	
	delete staticInstancePointer_;
	staticInstancePointer_=0;
	}


//*** GetInstance ***

template <class TYPE> 
TYPE* Singleton<TYPE>::GetInstance()
	{
	if (!staticInstancePointer_)
		{
		FatalError("Attempt to access a singleton instance before it's been created.");
		}
	
	return staticInstancePointer_;
	}


//*** IsInstanceCreated ***

template <class TYPE> 
bool Singleton<TYPE>::IsInstanceCreated()
	{
	if (!staticInstancePointer_)
		{
		return false;
		}
	
	return true;
	}

