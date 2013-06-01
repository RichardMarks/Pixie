/**
 * \class	ResourceManager
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ResourceManager_H__
#define __ResourceManager_H__

// Includes
#include "StringId.h"
#include "HashTable.h"
#include "HashTableKey_StringId.h"
#include "HashTableKey_Pointer.h"
#include "HashTableIterator.h"
#include "Bag.h"
#include "BagIterator.h"
#include "Singleton.h"

// External classes

// ResourceManager
class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		/**
		 * Destructor. All resources must be released before we exit, so just assert that this has been done. 
		 */
		~ResourceManager()
			{
			Assert(resourcesByFilename_.GetItemCount()==0 && entriesByPointer_.GetItemCount()==0 && resourcesByType_.GetItemCount()==0,
				"Some of the allocated resources was not released before the resource manager shut down.");
			}

		/**
		 * Retrieves a list (in no particular order) of resources of the specified type.
		 */
		template<typename T> const Bag<T*>& GetResourceList() const;

		/**
		 * Retrieves the filename of the specified resource
		 */
		template<typename T> StringId GetFilename( 
			T* pointer ///< Pointer to the resource to receive the name of
			) const;

	private:
		template<typename T> friend class Resource;

		/**
		 * Retrieves the pointer of the resource with the specified filename. If the resource
		 * with that filename has not been created before, the resource manager will create
		 * it and return a pointer to it, but if it already exists, it will just return it.
		 */
		template<typename T> void GetPointer( 
			StringId filename, ///< Filename of the resource
			T* &pointer ///< Reference to the pointer variable to receive the result
			);

		/**
		 * Increases the reference count of the specified resource
		 */
		template<typename T> void AddReference( 
			T* pointer ///< Resource to increase the reference count for
			);

		/**
		 * Decreases the reference count of the specified resource. If the reference count reaches
		 * zero, the resource will be destroyed, and removed from the resource manager
		 */
		template<typename T> void RemoveReference( 
			T* pointer ///< Resource to decrease the reference count for 
			);


	private:
		/**
		 * Structure for storing information about each resource
		 */
		struct Entry
			{
			StringId filename;
			int referenceCount;
			const char* type;
			};

		HashTable<HashTableKey_StringId,void*> resourcesByFilename_; ///< Resource pointers, indexed by filename
		HashTable<HashTableKey_Pointer,Entry> entriesByPointer_; ///< Resource information, indexed by resource pointer
		HashTable<HashTableKey_Pointer,void*> resourcesByType_; ///< Lists for each resource type, indexed by type name
	};


// Implementation	
#include "ResourceManager.inl"

#endif /* __ResourceManager_H__ */
