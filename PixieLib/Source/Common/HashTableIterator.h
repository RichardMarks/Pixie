/**
 * \class	HashTableIterator
 * 
 * \ingroup	containers
 * \brief	Iterator for HashTable
 * \author	Mattias Gustavsson
 * 
 * The iterator provides a convenient way to loop trough 
 * all the items of a hash table in the same way as
 * you would a linked list. When created, the iterator 
 * points at the first item in the table, and you can 
 * then move it around using GetFirst, GetNext, GetLast
 * and GetPrevious, and even search for a specific item
 * using Find. Note that hash tables are optimized to be
 * extremely fast when searching (using Find), but this also
 * results in stepping through the table using GetFirst, 
 * GetNext etc. might be very slow, and it is not recommended
 * that you do this in performance critical code. If you need
 * fast looping through, consider using a linked list or a
 * dynamic array instead (which are much less efficient when 
 * using Find). 
 */

#ifndef __HashTableIterator_H__
#define __HashTableIterator_H__

// Includes

// Forward declares
template<class HASHTABLEKEY, class TYPE> class HashTable;

// HashTableIterator
template<class HASHTABLEKEY, class TYPE> 
class HashTableIterator
	{
	public:
		/**
		 * Constructor
		 */
		HashTableIterator(const HashTable<HASHTABLEKEY,TYPE>& table);
	
		/**
		 * Returns the first item in the table, and resets the iterator
		 * to return the second item the next time GetNext is called
		 *
		 * \returns The first item in the table
		 */
		void MoveFirst();

		/**
		 * Returns the next item in the table, and advances the iterator
		 *
		 * \returns The next item in the table, or 0 if there is none
		 */
		void MoveNext();

		/**
		 * Returns the previous item in the table, and moves the iterator back
		 *
		 * \returns The previous item in the table, or 0 if there is none
		 */
		void MovePrevious();

		/**
		 * Returns the last item in the table, and resets the iterator
		 * to return the next to last item the next time GetPrevious is called
		 *
		 * \returns The last item in the table
		 */
		void MoveLast();
		
		/**
		 * Returns the item the iterator is currently pointing at
		 *
		 * \returns The current item 
		 */
		TYPE& GetCurrent() const;

		/**
		 * Checks if the iterator have reached the end of the table
		 *
		 * \returns True if the iterator is at the end of the table
		 */
		bool IsValid() const;


		/**
		 * Finds an item in the table
		 *	
		 * \returns A pointer to the item, or 0 if it was not found
		 */
		bool Find(
			const HASHTABLEKEY& key ///< Key of item to look for
			);

		/**
		 * Gets the key of the item that the iterator is currently pointing at
		 *	
		 * \returns Key of the item the iterator is currently pointing at
		 */
		const HASHTABLEKEY& GetCurrentKey() const;

	private:
		friend class HashTable<HASHTABLEKEY, TYPE>;
		int	currentGetIndex_;	///< Index of the current item of the GetFirst()/GetNext() operations
		const HashTable<HASHTABLEKEY, TYPE>* table_;	///< The table that this iterator is used to iterate through
	};
	
// Implementation	
#include "HashTableIterator.inl"

#endif /* __HashTableIterator_H__ */
	