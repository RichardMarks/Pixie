/**
 * \class	MemoryPool
 * 
 * \ingroup	containers
 * \brief	MemoryPool for faster dynamic memory allocation
 * \author	Mattias Gustavsson	
 * 
 * Memory pools are very useful if you need to dynamically allocate and free many blocks of 
 * memory that are all the same size. A memory pool works by allocating larger chunks of 
 * memory, and dishing it out in smaller chunks as requested. When memory is freed, it
 * is not released, but instead recycled to be used in future requests to allocate memory.
 * Another advantage of memory pools is that blocks are kept closer to each other in memory,
 * increasing cache coherency.
 */

#ifndef __MemoryPool_H__
#define __MemoryPool_H__

// Includes

// Forward declares

// MemoryPool
template<class TYPE> 
class MemoryPool
	{
	public:
		/**
		 * Constructor
		 */
		MemoryPool(
			int entriesPerBlock = 64	///< The number of entries per block. By tweaking this value, better performance might be obtained. Things that rarely allocates from the memory pool can have a low value for this, but things doing many allocations are better off upping it. 
			);

		/**
		 * Destructor
		 */
		~MemoryPool();

		/**
		 * Retrieves an unused entry from the memory pool. If there are no more 
		 * unused entries left, a new block will be allocated.
		 *
		 * \returns	A pointer to an unused entry from the memory pool
		 */
		TYPE* Allocate();

		/**
		 * Marks the specified entry contained in the memory pool as unused
		 */
		void Free(
			TYPE* object	///< Memory pool entry to release
			);

		/**
		 * Determines if the memory pool is still in use, meaning that there are still
		 * entries of the pool being used/allocated.
		 *
		 * \returns True if any of the entries of the memory pool is marked as used, False otherwise
		 */
		bool IsInUse() const;

	private:
		/**
		 * Creates a new block and then setups the pointer entries.
		 */
		void CreateBlock();

	private:
		int entriesPerBlock_;	///< Number of entries stored in each block
		int usedEntriesCount_;	///< Number of currently used entries

		TYPE** entries_;	///< Array holding all unused entries
		int entriesSize_;	///< Size of the entries array
		int entriesCount_;	///< Number of entries in the entries array

		TYPE** blocks_;	///< Array holding all allocated blocks
		int blocksSize_;	///< Size of the blocks array
		int blocksCount_;	///< Number of blocks in the blocks array

	};

// Implementation
#include "MemoryPool.inl"

#endif /* __MemoryPool_H__ */