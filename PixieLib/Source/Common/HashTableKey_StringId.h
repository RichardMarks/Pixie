/**
 * \class	HashTableKey_StringId
 * 
 * \ingroup	containers
 * \brief	Hash table key implementation for string keys
 * \author	Mattias Gustavsson	
 * 
 * Hash table key implementation for string keys
 */

#ifndef __HashTableKey_StringId_H__
#define __HashTableKey_StringId_H__

// Includes
#include "HashTableKey.h"

// Forward declares

// HashTableKey_StringId
class HashTableKey_StringId: public HashTableKey
	{
	public:
		HashTableKey_StringId();

		/**
		 * Constructor
		 */
		HashTableKey_StringId(
			StringId stringId	///< String value that this key should represent
			);
			
		
		/**
		 * Used to find out the specific type of HashTableKey that this class implements
		 *
		 * \returns	A string id ("HashTableKey_StringId") uniquely identifying this key type
		 */ 			
		virtual StringId GetType() const;		

		/**
		 * Gets the hash number for this key
		 * 
		 * \returns The hash number for this key
		 */
		unsigned int GetHash() const;


		/**
		 * Compares this key with the specified key. 
		 *
		 * \returns	True if the two keys are identical, false if they are not
		 */
		bool Compare(
			const HashTableKey* key
			) const;
		
		/**
		 * Gets the actual string id used for this key
		 *
		 * \returns	The string id used for this key
		 */
		StringId GetStringId() const;
	
	private:	
		StringId stringId_;	///< Stores the actual stringId used for this key
	
	};


#endif /* __HashTableKey_StringId_H__ */