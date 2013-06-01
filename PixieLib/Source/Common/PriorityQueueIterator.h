/**
 * \class	PriorityQueueIterator
 * 
 * \ingroup	containers
 * \brief	Iterator for PriorityQueue
 * \author	Mattias Gustavsson
 * 
 */

#ifndef __PriorityQueueIterator_H__
#define __PriorityQueueIterator_H__

// Includes

// Forward declares
template <typename TYPE, class COMPARE> class PriorityQueue;

// PriorityQueueIterator
template <typename TYPE, class COMPARE> 
class PriorityQueueIterator
	{
	public:
		/**
		 * Constructor
		 */
		PriorityQueueIterator(const PriorityQueue<TYPE,COMPARE>& priorityQueue);
	
		/**
		 * Returns the first item in the queue, and resets the iterator
		 * to return the second item next time GetNext is called
		 *
		 * \returns The first item in the queue
		 */
		void MoveFirst();

		/**
		 * Returns the next item in the queue, and advances the iterator
		 *
		 * \returns The next item in the queue
		 */
		void MoveNext();

		/**
		 * Returns the previous item in the queue, and moves the iterator back
		 *
		 * \returns The previous item in the queue
		 */
		void MovePrevious();

		/**
		 * Returns the last item in the queue, and resets the iterator
		 * to return the second last item next time GetPrevious is called
		 *
		 * \returns The last item in the queue
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
		 * Finds an item in the queue
		 *	
		 * \returns True if the item was found, or false if it was not found
		 */
		bool Find(
			 const TYPE& data	///< item to look for
			);

		/**
		 * Gets the index that the iterator is currently pointing at
		 *	
		 * \returns Index of the item the iterator is currently pointing at
		 */
		int GetCurrentIndex() const;

	private:
		int get_;				///< Index of the current item of the GetFirst()/GetNext() operations
		const PriorityQueue<TYPE,COMPARE>* queue_;	///< The queue that this iterator is used to iterate through
	};
	
// Implementation	
#include "PriorityQueueIterator.inl"

#endif /* __PriorityQueueIterator_H__ */
	