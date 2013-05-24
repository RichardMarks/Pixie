/**
 * \class	HashTableKey_String
 * 
 * \ingroup	containers
 * \brief	Hash table key implementation for string keys
 * \author	Mattias Gustavsson	
 * 
 * Hash table key implementation for string keys
 */

#ifndef __HashTableKey_String_H__
#define __HashTableKey_String_H__

// Includes
#include "HashTableKey.h"

// Forward declares

// HashTableKey_String
class HashTableKey_String: public HashTableKey
	{
	public:
		HashTableKey_String();

		HashTableKey_String(
			const HashTableKey_String& string	///< String value that this key should represent
			);


		const HashTableKey_String& operator = (
			const HashTableKey_String& string	///< String value that this key should represent
			);
		/**
		 * Constructor
		 */
		HashTableKey_String(
			const char* string	///< String value that this key should represent
			);
			
		/**
		 * Destructor
		 */
		~HashTableKey_String();
		
		/**
		 * Used to find out the specific type of HashTableKey that this class implements
		 *
		 * \returns	A string id ("HashTableKey_String") uniquely identifying this key type
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
		 * Gets the actual string used for this key
		 *
		 * \returns	The string used for this key
		 */
		const char* GetString() const;
	
	private:
		/** 
		 * Calculates the hash value for the given string
		 *
		 * \returns The hash value for the given string
		 */
		 
		unsigned int CalculateHash(
			const char* key	///< String to calculate hash value for
			) const;

	private:	
		char* string_; ///< Stores the actual string used for this key
		unsigned int hash_;	///< Stores the hash value for the string (so we only have to calculate it once)
	
	};


#endif /* __HashTableKey_String_H__ */