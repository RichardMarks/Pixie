/**
 * \class	BagIterator
 * 
 * \ingroup	containers
 * \brief	Iterator for Bag
 * \author	Mattias Gustavsson
 * 
 * The iterator provides a convenient way to loop trough all the items of a Bag in the same way as you would a linked 
 * list. When created, the iterator points to the first item in the bag, and you can then move it around using 
 * MoveFirst, MoveNext, MoveLast and MovePrevious, and even search for a specific item using Find (note that such a 
 * search would be looping through all items, and that can be quite slow if the bag contains many items. For this type
 * of operation, consider using a HashTable instead, which provides very fast lookup of items).
 */

#ifndef __BagIterator_H__
#define __BagIterator_H__

// Includes

// Forward declares
template <class TYPE> class Bag;

// BagIterator
template <class TYPE> 
class BagIterator
	{
	public:
		/**
		 * Constructor
		 */
		BagIterator(const Bag<TYPE>& bag);
	
		/**
		 * Set the iterator to point at the first item in the bag.
		 */
		void MoveFirst();

		/**
		 * Advances the iterator to the next item in the bag. 
		 */
		void MoveNext();

		/**
		 * Moves the iterator back to the previous item in the bag. 
		 */
		void MovePrevious();

		/**
		 * Set the iterator to point at the last item in the bag.
		 */
		void MoveLast();
		
		/**
		 * Checks if the iterator is currently pointing at a valid location in the bag. If it returns true, it is
		 * safe to call GetCurrent and GetCurrentIndex.
		 *
		 * \returns True if the iterator is currently pointing at a valid item
		 */
		bool IsValid() const;

		/**
		 * Gets the item the iterator is currently pointing at. If the iterator is not currently pointing at a valid
		 * location, a default value will be returned, and in debug builds an assertion will be triggered.
		 *
		 * \returns The item the iterator is currently pointing at
		 */
		TYPE& GetCurrent() const;

		/**
		 * Gets the index that the iterator is currently pointing at. If the iterator is not currently pointing at a 
		 * valid location, this method will return either the value -1 (if the iterator has moved beyond the start of
		 * the bag) or a value equal to the number of items in the bag (if it has been moved beyond the end).
		 *	
		 * \returns Index that the iterator is currently pointing at
		 */
		int GetCurrentIndex() const;

		/**
		 * Finds an item in the bag, by looping through the bag and checking each item. If the item is found, this
		 * method will return true, and the item itself and its index can be retrieved by calling GetCurrent and 
		 * GetCurrentIndex, respectively.
		 *	
		 * \returns True if the item was found, or false if it was not found
		 */
		bool Find(
			 const TYPE& data	///< Item to look for
			);

	private:
		int get_;				///< Index of the current item of the GetFirst()/GetNext() operations
		const Bag<TYPE>* bag_;	///< The bag that this iterator is used to iterate through
	};
	
// Implementation	
#include "BagIterator.inl"

#endif /* __BagIterator_H__ */
	