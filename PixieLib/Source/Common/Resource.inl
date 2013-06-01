//*** Resource.cpp ***/

#include "Resource.h"

#include "ResourceManager.h"

//*** Default Constructor ***

template<typename T>
Resource<T>::Resource():
	instance_(0)
	{
	}


//*** Constructor ***

template<typename T>
Resource<T>::Resource( StringId filename ):
	instance_(0)
	{
	if ( ResourceManager::IsInstanceCreated() )
		{
		ResourceManager::GetInstance()->GetPointer( filename, instance_ );
		ResourceManager::GetInstance()->AddReference( instance_ );
		}
	}


//*** Copy Constructor ***

template<typename T>
Resource<T>::Resource( const Resource& resource):
	instance_(resource.instance_)
	{
	if ( ResourceManager::IsInstanceCreated() )
		{
		ResourceManager::GetInstance()->AddReference( instance_ );
		}
	}


//*** Destructor ***

template<typename T>
Resource<T>::~Resource()
	{
	if ( ResourceManager::IsInstanceCreated() )
		{
		ResourceManager::GetInstance()->RemoveReference( instance_ );
		}
	}


//*** Assignment operator ***

template<typename T>
const Resource<T>& Resource<T>::operator=(const Resource& resource)
	{
	if ( resource.instance_ != instance_ )
		{
		if ( ResourceManager::IsInstanceCreated() )
			{
			ResourceManager::GetInstance()->AddReference( resource.instance_ );
			ResourceManager::GetInstance()->RemoveReference( instance_ );
			}
		}
	instance_ = resource.instance_;
	return *this;
	}


//*** GetFilename ***

template<typename T>
StringId Resource<T>::GetFilename() const
	{
	StringId filename;
	if ( ResourceManager::IsInstanceCreated() )
		{
		filename = ResourceManager::GetInstance()->GetFilename( instance_ );
		}
	return filename;
	}


//*** GetInstance ***

template<typename T>
T* Resource<T>::GetInstance() const
	{
	return instance_;
	}
