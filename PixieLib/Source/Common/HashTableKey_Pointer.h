/**
 * \class	HashTableKey_Pointer
 * 
 * \ingroup	containers
 * \brief	Hash table key implementation for void* keys
 * \author	Mattias Gustavsson	
 * 
 * Hash table key implementation for void* keys. 
 */

#ifndef __HashTableKey_Pointer_H__
#define __HashTableKey_Pointer_H__

// Includes
#include "HashTableKey.h"

// Forward declares

// HashTableKey_Pointer
class HashTableKey_Pointer: public HashTableKey
	{
	public:
		HashTableKey_Pointer();

		/**
		 * Constructor
		 */
	
		 HashTableKey_Pointer(
			const void* pointer	///< The actual number to be used for this key
			);

		/**
		 * Used to find out the specific type of HashTableKey that this class implements
		 *
		 * \returns	A string id ("HashTableKey_Pointer") uniquely identifying this key type
		 */ 			
		virtual StringId GetType() const;		

		/**
		 * Gets the hash number for this key
		 * 
		 * \returns The hash number for this key
		 */
		 virtual unsigned int GetHash() const;


		/**
		 * Compares this key with the specified key. 
		 *
		 * \returns	True if the two keys are identical, false if they are not
		 */
		 bool Compare(const HashTableKey* key) const;

		/**
		 * Gets the actual number used for this key
		 *
		 * \returns	The number used for this key
		 */
		 unsigned int GetInt() const;

	private:	
		unsigned int hash_;	///< Stores the number used for this key (this is also the hash number for the key)
		const void* pointer_;
	};


#endif /* __HashTableKey_Pointer_H__ */