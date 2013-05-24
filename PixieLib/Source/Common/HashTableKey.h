/**
 * \class	HashTableKey
 * 
 * \ingroup	containers
 * \brief	Abstract base class for hash table keys
 * \author	Mattias Gustavsson	
 * 
 * The HashTable container may store data indexed by any type of key, as long as
 * there's an implementation for it derived from this abstract base class. The
 * HashTable class will accept any object derived from HashTableKey as a valid key
 * value, but will detect if different types of keys are used within the same
 * HashTable instance, and report an error if that is the case.
 * 
 * It is a good idea to create HashTableKeys during initialization or similar, and
 * store them off for later use, as the way they work is to calculate the hash 
 * value when they are created, and after that they're quite quick to work with.
 */

#ifndef __HashTableKey_H__
#define __HashTableKey_H__

// Includes
#include "StringId.h"

// Forward declares

// HashTableKey
class HashTableKey
	{
	public:

		/**
		 * Destructor
		 */
		virtual ~HashTableKey() { };

	
		/**
		 * The hashtable can store data with different types of keys, and doesn't need to 
		 * know which type it is to perform its functionality. However, things would not
		 * work very well if different types of keys were used within the same hash table.
		 * Therefore, each type of HashTableKey needs to implement the GetType method, to
		 * return a unique string id for that specific key type. It is only used to make
		 * sure that all keys used within the same hash table is of the same type. For the 
		 * actual string it is recommended that the actual class name is used, for example
		 * "HashTableKey_Int" or "HashTableKey_String".
		 *
		 * \returns	A pointer to a string id uniquely identifying this type of hash key.
		 */
		virtual StringId GetType() const = 0;		


		/**
		 * Gets the hash number for this key
		 * 
		 * \returns The hash number for this key
		 */
		virtual unsigned int GetHash() const = 0;
		
		/**
		 * Compares this key with the specified key. This might be a cheap operation
		 * if the actual key is simple (like for example the integer key type) or more
		 * expensive for complex keys (the string key type will perform a full string 
		 * compare (case insensitive) when comparing keys).
		 *
		 * Note that this method compare the actual key values, not their calculated 
		 * hash values.
		 *
		 * \returns	True if the two keys are identical, false if they are not
		 */
		virtual bool Compare(
			const HashTableKey* key	///< Key to compare this key to
			) const = 0;
		
	};

#endif /* __HashTableKey_H__ */
