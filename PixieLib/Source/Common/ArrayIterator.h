/**
 * \class	ArrayIterator
 * 
 * \ingroup	containers
 * \brief	Iterator for Array
 * \author	Mattias Gustavsson
 * 
 * The iterator provides a convenient way to loop trough all the items of an Array in the same way as you would a linked 
 * list. When created, the iterator points to the first item in the array, and you can then move it around using 
 * MoveFirst, MoveNext, MoveLast and MovePrevious, and even search for a specific item using Find (note that such a 
 * search would be looping through all items, and that can be quite slow if the array contains many items. For this type
 * of operation, consider using a HashTable instead, which provides very fast lookup of items).
 */

#ifndef __ArrayIterator_H__
#define __ArrayIterator_H__

// Includes

// Forward declares
template <class TYPE> class Array;

// ArrayIterator
template <class TYPE> 
class ArrayIterator
	{
	public:
		/**
		 * Constructor
		 */
		ArrayIterator(const Array<TYPE>& array);
	
		/**
		 * Set the iterator to point at the first item in the array.
		 */
		void MoveFirst();

		/**
		 * Advances the iterator to the next item in the array. 
		 */
		void MoveNext();

		/**
		 * Moves the iterator back to the previous item in the array. 
		 */
		void MovePrevious();

		/**
		 * Set the iterator to point at the last item in the array.
		 */
		void MoveLast();
		
		/**
		 * Checks if the iterator is currently pointing at a valid location in the array. If it returns true, it is
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
		 * the array) or a value equal to the number of items in the array (if it has been moved beyond the end).
		 *	
		 * \returns Index that the iterator is currently pointing at
		 */
		int GetCurrentIndex() const;

		/**
		 * Finds an item in the array, by looping through the array and checking each item. If the item is found, this
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
		const Array<TYPE>* array_;	///< The array that this iterator is used to iterate through
	};
	
// Implementation	
#include "ArrayIterator.inl"

#endif /* __ArrayIterator_H__ */
	