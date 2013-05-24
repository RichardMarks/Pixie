/**
 * \class	DynamicBuffer
 * 
 * \ingroup	containers
 * \brief	Dynamic memory buffer
 * \author	Mattias Gustavsson
 * 
 * The dynamic memory buffer is useful when you want to work with a memory buffer and not have to worry about buffer 
 * overruns or allocating/freeing memory. The dynamic buffer handles all that by itself. It will dynamically grow the 
 * buffer when needed, and you can safely use the Read/Write methods to manipulate its content, much like you would a
 * file, but with the speed that comes from using memory.
 *
 * It is also possible to access the memory of the buffer directly, but that can be risky as no safety checks are 
 * carried out, which opens it up to possible buffer overrun problems.
 *
 * Note that memory used by the buffer is not allocated until the first time data is written to it. If it is important 
 * to have the memory allocated when the buffer is created, just do Write/Clear to force the allocation.
 */

#ifndef __DynamicBuffer_H__
#define __DynamicBuffer_H__

// Includes

// Forward declares

// DynamicBuffer
class DynamicBuffer
	{
	public:
		/**
		 * Constructor
		 */
		DynamicBuffer(
			unsigned int initialCapacity=1024	///< Maximum number of bytes the buffer can initially store. Will be
												///< doubled every time the available space is used up.
			);
		
		/**
		 * Copy Constructor
		 */
		DynamicBuffer(
			const DynamicBuffer& dynamicBuffer	///< Buffer to copy
			);
		
		/**
		 * Assignment operator
		 *
		 *\returns	The new buffer
		 */
		const DynamicBuffer& operator = (
			const DynamicBuffer& dynamicBuffer	///< Buffer to copy
			);

		/**
		 * Destructor
		 */
		~DynamicBuffer();
		
		/**
		 * Removes all data from the buffer. Usually, when clearing a buffer, you know it to be quite likely that future 
		 * use of the buffer will result in about the same amount of space being required, and that is the default 
		 * behavior: the memory is being kept as it is, and only the members keeping track of how much of the buffer is 
		 * used will be reset. A little care needs to be taken though, as this might result in a lot of memory just 
		 * hanging around when it could be used for other things instead. If you know that you probably won't be using 
		 * as much memory in future uses of the buffer, you should specify true for the releaseMemory parameter, which 
		 * will result in all the memory allocated for the buffer to be released, and the buffer will be reset to its
		 * initial size. 
		 */
		void Clear(
			bool releaseMemory = false	///< If set to true, all memory used by the buffer will be released, and the 
										///< size is reset to the initial value
			);

		/**
		 * Gets the current size of the buffer
		 *
		 * \returns The size of the used part of the buffer, in bytes
		 */
		unsigned int GetSize() const;
		
		/**
		 * Sets the current size of the buffer, in bytes
		 */
		void SetSize(
			unsigned int size	///< The new size
			);

		/**
		 * Gets the maximum number of bytes that can currently be stored in the buffer before it has to be resized
		 *
		 * \returns The current capacity of the buffer, in bytes
		 */
		unsigned int GetCapacity() const;
		
		/**
		 * Sets the maximum number of bytes that can be stored in the buffer. If the new capacity is less than the
		 * current capacity, the capacity will remain at its current value. 
		 */
		void SetCapacity(
			unsigned int capacity	///< The new capacity
			);

		/**
		 * Gets the current position for read/write operations on the buffer. This is the position at which the next 
		 * subsequent read/write operation will be done.
		 *
		 * \returns The current position in the buffer
		 */
		unsigned int GetPosition() const;

		/**
		 * Sets the current position for read/write operations on the buffer. The next subsequent read/write operation 
		 * will be performed at this position. Note that if the position is set to a value that is outside the currently 
		 * used range of the buffer, the buffer will be resized on the next Write operation, if need be.
		 */
		void SetPosition(
			unsigned int position	///< The new position of the buffer
			);
	
		/**
		 * Writes data to the buffer. Will copy "count" number of chars from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const char* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);

		/**
		 * Writes data to the buffer. Will copy "count" number of shorts from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const short* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
				
		/**
		 * Writes data to the buffer. Will copy "count" number of ints from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const int* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of longs from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const long* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
	
		/**
		 * Writes data to the buffer. Will copy "count" number of unsigned chars from the memory location specified in 
		 * the "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const unsigned char* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of unsigned shorts from the memory location specified in 
		 * the "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const unsigned short* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of unsigned ints from the memory location specified in 
		 * the "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const unsigned int* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of unsigned longs from the memory location specified in 
		 * the "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const unsigned long* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of floats from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(
			const float* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of doubles from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */		
		unsigned int Write(
			const double* value, ///< Pointer to the first element to write
			unsigned int count = 1	///< Number of elements to write
			);
		
		/**
		 * Writes data to the buffer. Will copy "count" number of bools from the memory location specified in the 
		 * "value" pointer to the current write position of the buffer.
		 *
		 * \returns	The number of successfully written elements
		 */
		unsigned int Write(const bool* value, unsigned int count = 1);

		/**
		 * Reads data from the buffer. Will copy "count" number of chars from the current read position of the buffer to 
		 * the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			char* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of shorts from the current read position of the buffer 
		 * to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			short* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of ints from the current read position of the buffer to 
		 * the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			int* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of longs from the current read position of the buffer to 
		 * the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			long* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned chars from the current read position of the 
		 * buffer to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			unsigned char* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned shorts from the current read position of the 
		 * buffer to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			unsigned short* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned ints from the current read position of the 
		 * buffer to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			unsigned int* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of unsigned longs from the current read position of the 
		 * buffer to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			unsigned long* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of floats from the current read position of the buffer 
		 * to the memory location specified in  the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			float* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of doubles from the current read position of the buffer 
		 * to the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			double* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Reads data from the buffer. Will copy "count" number of bools from the current read position of the buffer to 
		 * the memory location specified in the "value" pointer
		 *
		 * \returns	The number of elements that where successfully read
		 */
		unsigned int Read(
			bool* value, ///< Pointer to the first element to read data into
			unsigned int count = 1	///< Number of elements to read
			);

		/**
		 * Used to retrieve a raw pointer to the actual memory that holds the buffer data. Use this carefully, because 
		 * no checks for memory overwrites or such will be done. Most of the time, the normal Read/Write methods should 
		 * be used.
		 */
		void* GetPointer() const;
				
	private:
		/**
		 * This method contains the code that is to be executed for all of the overloaded Write methods. The code is 
		 * identical for all the different Write methods, only the type of the input parameters change, so it makes more 
		 * sense to use a template method rather than copy-paste the code. 
		 */
		template <typename TYPE> unsigned int Write(
			const TYPE* value, ///< Pointer to the first element to write
			unsigned int count ///< Number of elements to write
			);

		/**
		 * This method contains the code that is to be executed for all of the overloaded Read methods. The code is 
		 * identical for all the different Read methods, only the type of the input parameters change, so it makes more 
		 * sense to use a template method rather than copy-paste the code. 
		 */
		template <typename TYPE> unsigned int Read(
			TYPE* value, ///< Pointer to the first element to read data into
			unsigned int count ///< Number of elements to read
			);

		/**
		 * This method will ensure that the buffer is large enough to have "size" number of bytes added to it. If there 
		 * is not enough room, the buffer will double in size as many times over as is needed to have "size" number of
		 * bytes added to it.
		 */
		void DynamicBuffer::Resize(
			unsigned int size	///< The number of bytes that the buffer must be able to have added to it
			);

	private:
		unsigned int initialCapacity_;	///< Maximum number of bytes the buffer can initially store 
		unsigned int capacity_;	///< Maximum number of bytes that can currently be stored in the buffer
		unsigned int size_;	///< Number of bytes currently stored in the buffer
		unsigned int position_;	///< The current position that we are reading from/writing to
		unsigned char* buffer_;	///< Pointer to the actual buffer in memory
	};

#endif /* __DynamicBuffer_H__ */
