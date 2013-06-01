/**
 * \class	PriorityQueue
 * 
 * \ingroup	containers
 * \brief	
 * \author	Mattias Gustavsson
 * 
 * \todo	Change allocation scheme to use malloc/free and placement new, to ensure only objects in use are created
 */

#ifndef __PriorityQueue_H__
#define __PriorityQueue_H__

// Includes

// Forward declares

//	class ExampleCompare
//		{
//		public:
//			/**
//			 * Returns true if value a is higher priority than value b
//			 */
//			static bool Compare(const int& a, const int& b)
//				{
//				return (a > b);
//				}
//		};

// PriorityQueue
template<typename TYPE, class COMPARE> 
class PriorityQueue
	{
	public:
		/**
		 * Constructor
		 */
		PriorityQueue(	
			int initialCapacity = 64	///< Maximum number of items the PriorityQueue can initially store. 
			);
		
		/**
		 * Copy Constructor
		 */
		PriorityQueue(	
			const PriorityQueue<TYPE,COMPARE>& priorityQueueToCopy
			);

		/**
		 * Assignment operator
		 */
		const PriorityQueue<TYPE,COMPARE>& operator = (	
			const PriorityQueue<TYPE,COMPARE>& priorityQueueToCopy
			);

		/**
		 * Destructor
		 */
		~PriorityQueue();

		/**
 		 * Adds an item to the PriorityQueue. 
 		 */
		TYPE& Add(
			const TYPE& item
			);
			
		/**
		 * Removes an item from the PriorityQueue. 
		 */
		TYPE Remove();
		
		
		/**
		 * 
		 */
		void Update(int index);

		/**
		 *
		 */
		bool ItemExists(
			const TYPE& item
			) const;

		/**
		 * Gets the number of items in the PriorityQueue
		 *
		 * \returns Number of items currently being stored in the PriorityQueue
		 */
		int GetItemCount() const;

		/**
		 * Gets item at specified index
		 *
		 * \returns item at specified index
		 */
		TYPE& Get(
			int index	///< Index of item to get
			) const;

		int GetCapacity() const;

		void SetCapacity(int capacity);

		/**
		 * Removes all items from the PriorityQueue
		 */
		void Clear(
			bool releaseMemory=true	///< If set to true, all memory used by the PriorityQueue will be released, and the maximum number of items that can be stored is reset to the initial value
			);

	private:
		int initialCapacity_;	///< Maximum number of items the array can initially store. 
		int capacity_;		///< Maximum number of items that can currently be stored in the array
		int itemCount_;		///< Number of items currently stored in the array
		TYPE* items_;		///< Pointer to the actual array data
	};

// Implementation
#include "PriorityQueue.inl"

#endif /* __PriorityQueue_h__ */
