/**
 * \class	Array
 * 
 * \ingroup	containers
 * \brief	Generic dynamic array
 * \author	Mattias Gustavsson
 * 
 * This class store items in a dynamic array, which can grow in size as needed. Every time the array is full, it is 
 * resized to twice the size. There is also an iterator available, which supports a generic iterator interface of 
 * MoveFirst, MoveNext etc. so the dynamic array can also be used like a linked list. 
 * Note though, that removing/inserting items in  the middle of the array will cause all items after that item to be 
 * moved one slot, which could potentially be a problem if you have a huge array and often need to do those operations. 
 * For most typical cases though, it's not an issue, as the memory fragmentation and cache-misses you're likely to get 
 * with linked lists typically make them much slower than using an array with reshuffling on insertion/removal.
 * Note that memory used for storing items in the array is not allocated until the first time an item is added to the
 * array - if it is important to have the memory allocated when the array is created, just do Add/RemoveLast to fix it.
 */

#ifndef __Array_H__
#define __Array_H__

// Includes

// Forward declares
template <class TYPE> class ArrayIterator;

// Array
template<class TYPE> 
class Array
	{
	public:
		/**
		 * Constructor
		 */
		Array(	
			int initialCapacity = 64	///< Maximum number of items the array can initially store. Will be doubled 
										///< every time the available space is used up.
			);
		
		/**
		 * Copy Constructor
		 */
		Array(	
			const Array<TYPE>& arrayToCopy	///< Array to copy all items from
			);

		/**
		 * Assignment operator
		 *
		 *\returns	The new array
		 */
		const Array<TYPE>& operator = (	
			const Array<TYPE>& arrayToCopy	///< Array to copy all items from
			);

		/**
		 * Destructor
		 */
		~Array();
		
		/**
 		 * Adds an item to the end of the array, and resizes the array if necessary. This is the fastest way to add an 
		 * item to the array, and if the capacity of he array is large enough to hold the additional item, so that no 
		 * resizing has to be done, very little work is done.
		 */
		TYPE& Add(
			const TYPE& item = TYPE()	///< Item to insert at the end of the array
			);

		/**
 		 * Inserts an item into the array. The new item will be inserted before the item with the specified index, and 
		 * all items after it will be moved one slot. The array will be resized if necessary. If the specified index is
		 * not within valid range, the item will not be inserted, and in debug builds an assertion is triggered.
 		 */
		TYPE& InsertBefore(
			int index,	///< Index before which the new item will be inserted. 
			const TYPE& item = TYPE()	///< Item to insert
			);
			
		/**
 		 * Inserts an item into the array. The new item will be inserted before the item indicated by the iterator, and 
		 * all items after it will be moved one slot. The array will be resized if necessary. If the specified position
		 * is not within valid range, the item will not be inserted, and in debug builds an assertion is triggered.
 		 */
		TYPE& InsertBefore(
			const ArrayIterator<TYPE>& insertBefore,	///< Iterator indicating the item before which to insert the new one
			const TYPE& item = TYPE()	///< Item to insert
			);

		/**
 		 * Inserts an item into the array. The new item will be inserted after the item with the specified index, and 
		 * all items after it will be moved one slot. The array will be resized if necessary. If the specified index is
		 * not within valid range, the item will not be inserted, and in debug builds an assertion is triggered.
 		 */
		TYPE& InsertAfter(
			int index,	///< Index after which the new item will be inserted. 
			const TYPE& item = TYPE() ///< Item to insert

			);

		/**
 		 * Inserts an item into the array. The new item will be inserted after the item indicted by the iterator, and 
		 * all items after it will be moved one slot. The array will be resized if necessary. If the specified position
		 * is not within valid range, the item will not be inserted, and in debug builds an assertion is triggered.
 		 */
		TYPE& InsertAfter(
			const ArrayIterator<TYPE>& insertAfter,	///< Iterator indicating the item after which to insert the new one
			const TYPE& item = TYPE()	///< Item to insert
			);

		/**
		 * Removes the last item from the array. This is a very fast way to remove an item from the array. If there are
		 * no items in the array, this method doesn't do anything (though in debug builds and assertion is triggered).
		 */
		void RemoveLast();	
		
		/**
		 * Removes an item from the array, specified by index. Moves all the items that come after it one slot to fill
		 * the slot of the removed item. If the specified index is not within valid range, no item will be removed, and 
		 * in debug builds an assertion is triggered.
		 */
		void Remove(
			int index	///< Item to remove
			);

		/**
		 * Removes an item from the array, as indicated by the specified iterator. Moves all the items that come after 
		 * it one slot to fill the slot of the removed item. If the position is not within valid range, no item will be 
		 * removed, and in debug builds an assertion is triggered.
		 */
		void Remove(
			const ArrayIterator<TYPE>& iterator	///< Iterator indicating the item to remove
			);
		
		/**
		 * Gets the number of items in the array
		 *
		 * \returns Number of items currently being stored in the array
		 */
		int GetItemCount() const;

		/**
		 * Gets the item at the specified index. If the specified index is not within valid range, a default value is
		 * returned, and in debug builds an assertion is triggered.
		 *
		 * \returns Item at specified index
		 */
		TYPE& Get(
			int index	///< Index of item to get
			) const;

		/**
		 * Gets the item at the location indicated by the specified iterator. If the iterator is not within valid range, 
		 * a default value is returned, and in debug builds an assertion is triggered.
		 *
		 * \returns Item at location indicated by iterator
		 */
		TYPE& Get(
			const ArrayIterator<TYPE>& iterator	///< Iterator indicating the item to get
			) const;

		/**
		 * Sets item at specified index. If the index is larger than the current number of items, the number of items 
		 * will be adjusted. If the index is larger than the capacity of the array, the array will be resized to be 
		 * large enough to cope with the specified index. If index < 0, the item will not be set, and in debug builds an
		 * assertion will be triggered.
		 */
		void Set(
			int index,	///< Index of item to set
			const TYPE& item	///< Item to set at the specified index
			);
			
		/**
		 * Sets item indicated by the specified iterator. If the index is larger than the current number of items, the 
		 * number of items will be adjusted. If the index is larger than the capacity of the array, the array will be 
		 * resized to be large enough to cope with the specified index. If index < 0, the item will not be set, and in 
		 * debug builds an assertion will be triggered.
		 */
		void Set(
			const ArrayIterator<TYPE>& iterator,	///< Iterator indicating the location to set
			const TYPE& item	///< Item to set at the specified location
			);
			
		/**
		 * Gets the maximum number of items that can be stored in the array
		 *
		 * \returns	The current capacity of the array
		 */
		int GetCapacity() const;

		/**
		 * Sets the maximum number of items that can be stored in the array. If the new capacity is <= 0, the 
		 * capacity will remain unchanged, and in debug builds an assertion will be triggered. If the new capacity is 
		 * less than the current number of items, the array will be truncated to the new capacity.
		 */
		void SetCapacity(int capacity);
			
		/**
		 * Removes all items from the array
		 */
		void Clear(
			bool releaseMemory = false	///< If set to true, all memory used by the array will be released, and the 
										///< capacity is reset to the initial value
			);

		/**
		 * Loops through the array and checks for the specified item. If it is found, it returns true.
		 *
		 * \returns	True if the item exists, false it it doesn't
		 */
		bool ItemExists(
			const TYPE& item	///< Item to look for
			) const;

		/**
		 * Gets a pointer to the array data. This should be used carefully, and only when performance is really, really
		 * crucial. In most cases, the Get/Set methods are sufficient, and worth using as they are safe and won't crash.
		 *
		 * \returns A pointer to the array data
		 */
		TYPE* GetPointer() const;
		
		
	private:
		int initialCapacity_;	///< Maximum number of items the array can initially store
		int capacity_;		///< Maximum number of items that can currently be stored in the array
		int itemCount_;		///< Number of items currently stored in the array
		TYPE* items_;		///< Pointer to the actual array data
	};

// Implementation
#include "Array.inl"

#endif /* __Array_h__ */
