/**
 * \class	Bag
 * 
 * \ingroup	containers
 * \brief	A bag provides a fast way to store a collection of items when order doesn't matter
 * \author	Mattias Gustavsson
 * 
 * This class store items in a dynamic array, which can grow in size as needed. The order of the items is unspecified, 
 * and, might change when items are removed from it. Every time the bag is full, it is resized to twice the size. 
 * There is also an iterator available, which supports a generic iterator interface of MoveFirst, MoveNext etc.  
 * When an item is removed from the bag, the last item of the array will be moved to the location of the removed item,
 * and when an item is added, it is added to the end of the array. This means that both adding and removing items are
 * fast operations working at constant speed.
 * Note that memory used for storing items in the bag is not allocated until the first time an item is added to the
 * bag- if it is important to have the memory allocated when the bag is created, just do Add/Remove to fix it.
 */

#ifndef __Bag_H__
#define __Bag_H__

// Includes

// Forward declares
template <class TYPE> class BagIterator;

// Bag
template<class TYPE> 
class Bag
	{
	public:
		/**
		 * Constructor
		 */
		Bag(	
			int initialCapacity = 64	///< Maximum number of items the bag can initially store. Will be doubled 
										///< every time the available space is used up.
			);
		
		/**
		 * Copy Constructor
		 */
		Bag(	
			const Bag<TYPE>& bagToCopy	///< Bag to copy all items from
			);

		/**
		 * Assignment operator
		 *
		 *\returns	The new bag
		 */
		const Bag<TYPE>& operator = (	
			const Bag<TYPE>& bagToCopy	///< Bag to copy all items from
			);

		/**
		 * Destructor
		 */
		~Bag();
		
		/**
 		 * Adds an item to the bag, and resizes the bag if necessary. This is a very fast operation if the capacity of 
		 * the bag is large enough to hold the additional item.
		 */
		TYPE& Add(
			const TYPE& item = TYPE()	///< Item to insert into the bag
			);

		/**
		 * Removes the last item from the bag. If there are no items in the bag, this method doesn't do anything (though 
		 * in debug builds and assertion is triggered).
		 */
		void RemoveLast();	
		
		/**
		 * Removes an item from the bag, specified by index. Moves the last item in the bag into the slot of the removed 
		 * item. If the specified index is not within valid range, no item will be removed, and in debug builds an 
		 * assertion is triggered.
		 */
		void Remove(
			int index	///< Item to remove
			);

		/**
		 * Removes an item from the bag, as indicated by the specified iterator. Moves the last item in the bag into the 
		 * slot of the removed item. If the specified index is not within valid range, no item will be removed, and in 
		 * debug builds an assertion is triggered.
		 */
		void Remove(
			const BagIterator<TYPE>& iterator	///< Iterator indicating the item to remove
			);
		
		/**
		 * Gets the number of items in the bag
		 *
		 * \returns Number of items currently being stored in the bag
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
			const BagIterator<TYPE>& iterator	///< Iterator indicating the item to get
			) const;

		/**
		 * Gets the maximum number of items that can be stored in the bag
		 *
		 * \returns	The current capacity of the bag
		 */
		int GetCapacity() const;

		/**
		 * Sets the maximum number of items that can be stored in the bag. If the new capacity is <= 0, the 
		 * capacity will remain unchanged, and in debug builds an assertion will be triggered. If the new capacity is 
		 * less than the current number of items, the bag will be truncated to the new capacity.
		 */
		void SetCapacity(int capacity);
			
		/**
		 * Removes all items from the bag
		 */
		void Clear(
			bool releaseMemory = false	///< If set to true, all memory used by the bag will be released, and the 
										///< capacity is reset to the initial value
			);

		/**
		 * Loops through the bag and checks for the specified item. If it is found, it returns true.
		 *
		 * \returns	True if the item exists, false it it doesn't
		 */
		bool ItemExists(
			const TYPE& item	///< Item to look for
			) const;

		/**
		 * Gets a pointer to the bag data. This should be used carefully, and only when performance is really, really
		 * crucial. In most cases, the Get/Set methods are sufficient, and worth using as they are safe and won't crash.
		 *
		 * \returns A pointer to the bag data
		 */
		TYPE* GetPointer() const;
		
		
	private:
		int initialCapacity_;	///< Maximum number of items the bag can initially store
		int capacity_;		///< Maximum number of items that can currently be stored in the bag
		int itemCount_;		///< Number of items currently stored in the bag
		TYPE* items_;		///< Pointer to the actual bag data
	};

// Implementation
#include "Bag.inl"

#endif /* __Bag_h__ */
