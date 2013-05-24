/**
 * \class	HashTableKey_Int
 * 
 * \ingroup	containers
 * \brief	Hash table key implementation for integer keys
 * \author	Mattias Gustavsson	
 * 
 * Hash table key implementation for integer keys. This is the simplest
 * form of hash key, and it uses the number passed in as hash value, so
 * for some scenarios the calling code might need to transform the value
 * in some way to make it a suitable hashing value. For example, this key
 * type will work for using object pointers as hash keys, by just casting
 * the pointer to an int. However, since objects are aligned on 4 byte
 * boundaries, we waste 3 out of 4 hash entries. If the value was divided
 * by 4 before being passed to HashTableKey_Int, we wouldn't have that 
 * problem. This hash key type is provided as a means to store things 
 * based on any kind of hashing value. If specific behavior is wanted,
 * it is good to implement a specific hashtable key type for that type of
 * key. For example, there could be a HashTableKey_Pointer, that would 
 * automatically do the divide by 4, as explained above, or do some even
 * more clever tricks, such as manipulating the bits of the value, to get
 * a more suitable hash value.
 */

#ifndef __HashTableKey_Int_H__
#define __HashTableKey_Int_H__

// Includes
#include "HashTableKey.h"

// Forward declares

// HashTableKey_Int
class HashTableKey_Int: public HashTableKey
	{
	public:
		HashTableKey_Int();

		/**
		 * Constructor
		 */
	
		HashTableKey_Int(
			unsigned int number	///< The actual number to be used for this key
			);

		/**
		 * Used to find out the specific type of HashTableKey that this class implements
		 *
		 * \returns	A string id ("HashTableKey_Int") uniquely identifying this key type
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
	};

#endif /* __HashTableKey_Int_H__ */