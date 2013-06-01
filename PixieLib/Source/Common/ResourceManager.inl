//*** ResourceManager.inl ***/

#include "ResourceManager.h"

#include <typeinfo>

//*** GetPointer ***

template<typename T> 
void ResourceManager::GetPointer( StringId filename, T* &pointer )
	{
	// Check if we've got a valid filename
	pointer = 0;	
	if (filename == StringId() )
		{
		return;
		}

	// Does this resource already exist?
	HashTableIterator<HashTableKey_StringId,void*> it(resourcesByFilename_);
	if ( it.Find(filename) )
		{
		// Pointers are stored as void* but we can cast it here
		pointer = static_cast<T*>( it.GetCurrent() );

		// If we run in a debug build, we can do an extra search to find the
		// additional information about this resource. We can use this to
		// assert that it was added as the same type of resource as is being
		// requested.
		#ifdef _DEBUG
		HashTableIterator<HashTableKey_Pointer,Entry> itE(entriesByPointer_);
		if (itE.Find(pointer))
			{
			Entry& entry=itE.GetCurrent();
			const char* type = typeid(T).name();
			Assert(entry.type == type, "Resource added multiple times as different type" );
			}
		#endif

		return;
		}

	// Resource doesn't exist, so create it now, and add it to the resource managers lists

	pointer = new T( filename.GetString() );
	resourcesByFilename_.Insert( filename, pointer );

	const char* type = typeid(T).name();
	
	Entry entry;
	entry.filename = filename;
	entry.referenceCount = 0;
	entry.type = type;
	entriesByPointer_.Insert( pointer, entry );

	// Also, add it to the list for the relevant resource type (and create that list if it doesn't exist)
	HashTableIterator<HashTableKey_Pointer,void*> itT(resourcesByType_);
	if ( itT.Find(HashTableKey_Pointer(type)) )
		{
		Bag<T*>* bag = (Bag<T*>*)itT.GetCurrent();
		bag->Add(pointer);
		}
	else
		{
		Bag<T*>* bag = new Bag<T*>();
		bag->Add(pointer);
		resourcesByType_.Insert(type,bag);
		}
	}


//*** AddReference ***

template<typename T> 
void ResourceManager::AddReference( T* pointer )
	{
	if (pointer)
		{
		HashTableIterator<HashTableKey_Pointer,Entry> it(entriesByPointer_);
		if (it.Find(pointer))
			{
			it.GetCurrent().referenceCount++;
			}
		}
	}


//*** RemoveReference ***

template<typename T> 
void ResourceManager::RemoveReference( T* pointer )
	{
	if (pointer)
		{
		HashTableIterator<HashTableKey_Pointer,Entry> it(entriesByPointer_);
		if (it.Find(pointer))
			{
			Entry& entry=it.GetCurrent();
			entry.referenceCount--;

			// If the resource count reaches 0, delete the resource, and remove it from the
			// resource managers lists
			if ( entry.referenceCount == 0 )
				{
				delete pointer;
				resourcesByFilename_.Remove(entry.filename);
				entriesByPointer_.Remove(it);
				const char* type = typeid(T).name();

				// Remove resource from the list for the relevant resource type 
				HashTableIterator<HashTableKey_Pointer,void*> itT(resourcesByType_);
				if ( itT.Find(type) )
					{
					Bag<T*>* bag = (Bag<T*>*)itT.GetCurrent();
					BagIterator<T*> itB( *bag );
					if ( itB.Find(pointer) )
						{
						bag->Remove(itB);
						if ( bag->GetItemCount() == 0 )
							{
							resourcesByType_.Remove(type);
							delete bag;
							}
						}
					}
				}
			}
		}
	}


//*** GetResourceList ***

template<typename T> 
const Bag<T*>& ResourceManager::GetResourceList() const
	{
	// Find the list of resources for this type
	const char* type = typeid(T).name();
	HashTableIterator<HashTableKey_Pointer,void*> itT(resourcesByType_);
	if ( itT.Find(type) )
		{
		// Return the list if it exists
		Bag<T*>* bag = (Bag<T*>*)itT.GetCurrent();
		return (*bag);
		}

	// If it doesn't exist, return an empty list
	static Bag<T*> emptyBag;
	return emptyBag;
	}


//*** GetFilename ***

template<typename T> 
StringId ResourceManager::GetFilename( T* pointer ) const
	{
	if (pointer)
		{
		HashTableIterator<HashTableKey_Pointer,Entry> it(entriesByPointer_);
		if (it.Find(pointer))
			{
			return it.GetCurrent().filename;
			}
		}

	return StringId();
	}
