/**
 * \class	StringPool
 *
 * \ingroup	core
 * \brief
 * \author	Mattias Gustavsson
 *
 * \todo	Faster implementation using hashtable for lookup, and storage blocks to reduce allocations
 */

#ifndef __StringPool_H__
#define __StringPool_H__

// Includes
#include "Bag.h"
namespace pixie {
// Forward declares

// StringPool
class StringPool
	{
	public:
		StringPool(int stringMinLength = 64, int stringStorageBlockSize = 1024);

		~StringPool();

		const char* GetPoolString(const char* uniqueString);

		int GetReferenceCount(const char* poolString);

		void IncreaseReferenceCount(const char* poolString);

		void DecreaseReferenceCount(const char* poolString);

	private:
		Bag<char*> strings_;
		};

}; // namespace pixie

#endif /* __StringPool_H__ */
