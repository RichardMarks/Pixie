	/**
 * \class	Resource
 * 
 * \ingroup	core
 * \brief	Templated resource class to handle shared references to resources
 * \author	Mattias Gustavsson	
 *
 * The resource class allows any type of object to be created through the resource manager, as long
 * as the class passed as a template parameter has a constructor which takes a const char* parameter
 * for the filename to create the resource from.
 * If the resource has not previously been created, the resource manager will create it when the
 * Resource is initialised. If it was already created, the resource manager will just return it, and
 * increase its reference count, so that a shared instance of the object will be used by all resources
 * referencing it. When all the resources referencing an object has been destroyed (and thus, the
 * reference count of the resource has reached 0), the resource manager will delete the object.
 */

#ifndef __Resource_H__
#define __Resource_H__

// Includes
#include "StringId.h"

// External classes

// Resource
template<typename T>
class Resource
	{
	public:
		/**
		 * Default constructor. Creates a resource which doesn't point to an object.
		 */
		Resource();

		/**
		 * Creates an object of the template parameter type from the file name, unless
		 * it already exists, in which case this resource is set to reference it.
		 */
		Resource( 
			StringId filename	///< Filename of resource to create. A plain const char* can be passed for this
			);

		/**
		 * Copy constructor. Increases the reference count of the resource.
		 */
		Resource( 
			const Resource& resource	///< Resource to copy
			);

		/**
		 * Destructor. Will cause reference count to be reduced, and if it reaches 0, the referenced
		 * object will be deleted.
		 */
		~Resource();

		/**
		 * Assignment operator. Will decrease the reference count of this resource, and increase it on
		 * the resource passed in.
		 */
		const Resource& operator = ( 
			const Resource& resource	///< Resource to assign to this resource
			);

		/**
		 * Returns the filename that the resource was created from.
		 */
		StringId GetFilename() const;

		/**
		 * Returns a pointer to the actual shared object referenced by this resource
		 */
		T* GetInstance() const;


	private:
		T* instance_;	///< The object referenced by this resource
	};

// Implementation
#include "Resource.inl"

#endif /* __Resource_H__ */
