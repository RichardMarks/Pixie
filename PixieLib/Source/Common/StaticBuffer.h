/**
 * \class	StaticBuffer
 * 
 * \ingroup	containers
 * \brief	Static memory buffer
 * \author	Mattias Gustavsson
 * 
 * The static memory buffer is useful when you want to read data from piece of memory
 * in the same way as you would with a DynamicBuffer. The difference is that with a
 * StaticBuffer, the memory is passed in to the constructor, and no allocation or
 * copying of memory is done. Also, you can't write data to a StaticBuffer, only read.
 *
 * It is also possible to access the memory directly, but that can be risky as no safety
 * checks are carried out, which opens it up to possible buffer overrun problems.
 */

#ifndef __StaticBuffer_H__
#define __StaticBuffer_H__

// Includes
#include "DynamicBuffer.h"

// Forward declares

// StaticBuffer
class StaticBuffer
	{
	public:
		/**
		 * Constructor
		 */
		StaticBuffer(
			const DynamicBuffer& buffer
			);

		/**
		 * Constructor
		 */
		StaticBuffer(
			const void* buffer,
			int size
			);
		
		/**
		 * Destructor
		 */
		~StaticBuffer();
		

		/**
		 * Gets the current size of the buffer
		 *
		 * \returns The size of the used part of the buffer, in bytes
		 */
		int GetSize() const;
		
		/**
		 * Gets the current position for read operations on the buffer. This is
		 * the position at which the next subsequent read operation will be done.
		 *
		 * \returns The current position in the buffer
		 */
		int GetPosition() const;

		/**
		 * Sets the current position for read operations on the buffer. The next
		 * subsequent read operation will be performed at this position.
		 */
		void SetPosition(
			int position	///< The new position of the buffer
			) const;
	
		/**
		 * Reads data from the buffer. Will copy "count" number of chars from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			char* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of shorts from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			short* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of ints from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			int* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of longs from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			long* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned chars from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			unsigned char* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned shorts from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			unsigned short* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned ints from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			unsigned int* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned longs from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			unsigned long* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of floats from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			float* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of doubles from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			double* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Reads data from the buffer. Will copy "count" number of bools from the 
		 * current read position of the buffer to the memory location specified in 
		 * the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		int Read(
			bool* value, ///< Pointer to the first element to read data into
			int count = 1	///< Number of elements to read
			) const;

		/**
		 * Used to retrieve a raw pointer to the actual memory that holds the buffer data.
		 * Use this carefully, because no checks for buffer overruns or such will be done. 
		 * Most of the time, the normal Read methods should be used.
		 */
		const void* GetPointer() const;

	private:
		int size_;		
		mutable int position_;	///< The current position that we are reading from
		const unsigned char* buffer_;	///< Pointer to the actual buffer in memory
	};


#endif /* __StaticBuffer_H__ */
