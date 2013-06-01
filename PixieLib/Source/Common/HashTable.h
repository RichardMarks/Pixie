/**
 * \class	HashTable
 * 
 * \ingroup	containers
 * \brief	Stores data in a hash table, indexed by any type of key
 * \author	Mattias Gustavsson	
 * 
 * A hash table store items associated with a key, in such a way that you can quickly retrieve a value from the key.
 * Internally, the hash table stores the items in an array, and from the key it calculates a number (called a hash 
 * value), which is used to find which array entry to place the item in. This means there's no looping through the array
 * when finding an item, as the hash value is used to go straight into the right array entry.
 *
 * \todo	Change allocation scheme to use malloc/free and placement new, to ensure only objects in use are created
 * \todo	Add copy constructor/assignment operator
 */

#ifndef __HashTable_H__
#define __HashTable_H__

// Includes

// Forward declares
template<class HASHTABLEKEY, class TYPE> class HashTableIterator;

// HashTable
template<class HASHTABLEKEY, class TYPE> 
class HashTable
	{
	public:
		/**		
		 * Constructor
		 */
		HashTable(
			int initialSlots = 64	///< Number of slots initially in the table. Will grow when table is full.
			);

		
		/**		
		 * Destructor
		 */
		~HashTable();


		/**		
		 * Insert an item into the table, associating it with the specified key.		 
		 */
		TYPE& Insert(
			const HASHTABLEKEY& key,	///< Key to be used when searching the table for this item
			const TYPE& data			///< Item to be added.
			);
		
		/**
		 * Removes the item with the specified key from the table. 
		 */
		void Remove(
			const HASHTABLEKEY& key 		///< Key of the item to be removed
			); 

		/**
		 * Removes an in
		 */
		void Remove(
			const HashTableIterator<HASHTABLEKEY,TYPE>& iterator	///< item to remove
			); 


		/**	
		 * Retrieves the total number of items currently in the table
		 *
		 * \returns	Number of items in the table.
		 */
		int	GetItemCount() const;


		/**
		 * Removes all entries from the table. It is up to the application to destroy 
		 * the objects that are contained in the table.
		 */
		void Clear(
			bool releaseMemory = false	///< If set to true, all memory used by the table will be released, and the maximum number of items that can be stored is reset to the initial value
			);

	private:
		friend class HashTableIterator<HASHTABLEKEY,TYPE>;	///< It makes sense to have iterators being friends with the class they are iterating

		/// HashTableItem
		struct HashTableItem
			{
			HASHTABLEKEY key;	///< Key to be used when searching for this item
			TYPE data;	///< A pointer to the actual object that this item represents
			bool inUse;
			int itemCount;	///< How many items are stored in the table, that should have been in this slot

			HashTableItem():inUse(false),itemCount(0) {}
			};
	private:
		int initialSlots_; 		
		int slotCount_;	///< Total number of slots in the table
		int itemCount_;	///< Total number of items currently in the table
		HashTableItem* items_;	///< The actual table data
	};


// Implementation
#include "HashTable.inl"

#endif /* __HASHTABLE_H__ */
