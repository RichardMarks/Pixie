/**
 * \class	StringIdTable
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * Efficient way of storing string id's
 */

#ifndef __StringIdTable_H__
#define __StringIdTable_H__

// Includes

// Forward declares

// StringIdTable
class StringIdTable
	{
	private:
		// The StringIdTable is only used internally by the StringId class
		friend class StringId;

		/**
		 * We only ever have one instance of the StringIdTable, and it is
		 * created when the table is first used, and destroyed when the
		 * application shuts down. This method will return the internally
		 * stored instance of the StringIdTable, or create it if it does
		 * not already exist
		 *
		 * \returns The StringIdTable instance
		 */
		static StringIdTable& GetInstance();

		/**
		 * Constructor. This is meant to be private, as it is only the 
		 * GetInstance method which will create an instance of this class.
		 */
		StringIdTable();

		/**
		 * Destructor
		 */
		~StringIdTable();

		/**
		 * Looks up the specified string in the shared string table, using the
		 * pre-calculated hash-value for faster lookup. If the string is not 
		 * found in the table, it is inserted, and the shared string pointer
		 * corresponding to the specified idString is returned.
		 *
		 * \returns	The shared pointer for the specified id-string
		 */
		const char* FindIdString(
			const char* idString	///< The idString to find in or insert into the string table
			);

	private:
		/**
		 * Calculates a hash value for the specified string, by looping
		 * through all characters and performing some bit-wise operations
		 * to get a good distribution of hash values across a wide selction
		 * of string data.
		 *
		 * \returns	The hash value for the specified string
		 */
		unsigned int CalculateHash(
			const char* idString,	///< String to calculate hash value for
			int* stringLength ///< To receive the length of the string
			) const;

		/**
		 * Stores a copy of the specified string in the pre-allocated string storage
		 * block. Will allocate an additional block if the current block is full.
		 * The hash number of the string will be pre-appended to the string, so we
		 * don't have to recalculate it when the table needs re-hashing.
		 *
		 * \returns	A pointer to the copy
		 */
		char* StoreString(
			unsigned int hash, ///< Pre-calculated hash number for the string
			const char* string,	///< String to store a copy of
			int length ///< The length of the string
			);

		// The following will generate a compiler error if there is an attempt to create a second instance of the class
		StringIdTable(const StringIdTable&);	///< Copy constructor, not implemented, only accessible from within the class 
		const StringIdTable& operator=(const StringIdTable&);	///< Copy assignment, not implemented, only accessible from within the class 


	private:
		int idStringTableSlots_; ///< The total number of slots in the table
		int idStringTableItemCount_; ///< The number of strings stored in the table
		char** idStringTable_;	///< Table for storing all the idStrings
		
		
		static const int stringStorageBlockSize_=4*1024; ///< Strings are stored in pre-allocated blocks, and this specifies the size, in bytes, of each block. Whenever a block is full, a new block is allocated off the heap

		/// Each pre-allocated string block keeps track of two pointers
		struct StringStorageBlock
			{
			char* head; ///< Pointer to the originally allocated block of memory
			char* tail; ///< Pointer to the first free space of the block
			};

		StringStorageBlock* stringStorageBlocks_; ///< Array for storing the currently allocated string storage blocks
		int stringStorageBlockMaxCount_;	///< The maximum number of entries that can be stored in stringStorageBlocks_
		int stringStorageBlockCount_; ///< The current number of entries  stored in stringStorageBlocks_
		};

#endif /* __StringIdTable_H__ */
