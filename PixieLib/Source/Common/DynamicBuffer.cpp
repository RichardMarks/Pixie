//*** DynamicBuffer.cpp ***

#include "DynamicBuffer.h"
#include "Debug.h"
#include "StandardLibrary.h"


//*** Constructor ***

DynamicBuffer::DynamicBuffer(unsigned int initialCapacity):
	buffer_(0),
	initialCapacity_(initialCapacity),
	capacity_(initialCapacity),
	size_(0),
	position_(0)
	{

	}	


//*** Destructor ***

DynamicBuffer::~DynamicBuffer()
	{
	// Free memory used by the buffer
	if (buffer_)
		{
		Free(buffer_);
		buffer_=0;
		}
	}


//*** Copy Constructor ***

DynamicBuffer::DynamicBuffer(const DynamicBuffer& dynamicBuffer):
	buffer_(0),
	initialCapacity_(dynamicBuffer.initialCapacity_),
	capacity_(dynamicBuffer.capacity_),
	size_(dynamicBuffer.size_),
	position_(dynamicBuffer.position_)
	{
	if (dynamicBuffer.buffer_)
		{
		// Allocate memory
		buffer_=static_cast<unsigned char*>(Malloc(capacity_));
		Assert(buffer_,"Couldn't allocate memory for dynamic buffer");
		if (!buffer_)
			{
			FatalError("Allocation failed when allocating memory for dynamic buffer");
			}

		// Copy contents
		MemCpy(buffer_,dynamicBuffer.buffer_,size_);
		}
	}	


//*** Assignment operator ***

const DynamicBuffer& DynamicBuffer::operator =(const DynamicBuffer& dynamicBuffer)
	{
	// Free memory used by the buffer
	if (buffer_)
		{
		Free(buffer_);
		buffer_=0;
		}

	buffer_=0;
	initialCapacity_=dynamicBuffer.initialCapacity_;
	capacity_=dynamicBuffer.capacity_;
	size_=dynamicBuffer.size_;
	position_=dynamicBuffer.position_;

	if (dynamicBuffer.buffer_)
		{
		// Allocate memory
		buffer_=static_cast<unsigned char*>(Malloc(capacity_));
		Assert(buffer_,"Couldn't allocate memory for dynamic buffer");
		if (!buffer_)
			{
			FatalError("Allocation failed when allocating memory for dynamic buffer");
			}

		// Copy contents
		MemCpy(buffer_,dynamicBuffer.buffer_,size_);
		}

	return *this;
	}	


//*** Clear ***

void DynamicBuffer::Clear(bool releaseMemory)
	{	
	// Reset the position and size variables
	position_=0;
	size_=0;
	
	// Free allocated memory and reset to initial size if requested
	if (releaseMemory)
		{
		// Free memory used by the buffer
		if (buffer_)
			{
			Free(buffer_);
			buffer_=0;
			}
		
		// Reset to initial size
		capacity_=initialCapacity_;
		}
	}


//*** GetSize ***

unsigned int DynamicBuffer::GetSize() const
	{
	return size_;
	}


//*** SetSize ***

void DynamicBuffer::SetSize(unsigned int size)
	{
	size_=size;
	if (position_>size_)
		{
		position_=size_;
		}
	Resize(size-position_);										
	}


//*** GetCapacity ***

unsigned int DynamicBuffer::GetCapacity() const
	{
	return capacity_;
	}


//*** SetCapacity ***

void DynamicBuffer::SetCapacity(unsigned int capacity)
	{
	if (capacity_<capacity)
		{
		capacity_=capacity;

		if (buffer_)
			{	
			// Reallocate the actual buffer
			buffer_=static_cast<unsigned char*>(Realloc(buffer_,capacity_));
			Assert(buffer_,"Couldn't reallocate memory for dynamic buffer");
			if (!buffer_)
				{
				FatalError("Reallocation failed when allocating memory for dynamic buffer");
				}
			}
		}
	}


//*** GetPosition ***

unsigned int DynamicBuffer::GetPosition() const
	{
	return position_;
	}


//*** SetPosition ***

void DynamicBuffer::SetPosition(unsigned int position)
	{
	position_=position;
	}


//** Write methods */

unsigned int DynamicBuffer::Write(const char* value, unsigned int count) 
	{ 
	return Write<char>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const short* value, unsigned int count) 
	{ 
	return Write<short>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const int* value, unsigned int count) 
	{ 
	return Write<int>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const long* value, unsigned int count) 
	{ 
	return Write<long>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const unsigned char* value, unsigned int count) 
	{ 
	return Write<unsigned char>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const unsigned short* value, unsigned int count) 
	{ 
	return Write<unsigned short>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const unsigned int* value, unsigned int count) 
	{ 
	return Write<unsigned int>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const unsigned long* value, unsigned int count) 
	{ 
	return Write<unsigned long>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const float* value, unsigned int count) 
	{ 
	return Write<float>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const double* value, unsigned int count) 
	{ 
	return Write<double>(value,count);
	}
	
unsigned int DynamicBuffer::Write(const bool* value, unsigned int count) 
	{ 
	return Write<bool>(value,count);
	}




//** Read methods */
	
unsigned int DynamicBuffer::Read(char* value, unsigned int count)
	{
	return Read<char>(value,count);
	}

unsigned int DynamicBuffer::Read(short* value, unsigned int count)
	{
	return Read<short>(value,count);
	}

unsigned int DynamicBuffer::Read(int* value, unsigned int count)
	{
	return Read<int>(value,count);
	}

unsigned int DynamicBuffer::Read(long* value, unsigned int count)
	{
	return Read<long>(value,count);
	}

unsigned int DynamicBuffer::Read(unsigned char* value, unsigned int count)
	{
	return Read<unsigned char>(value,count);
	}

unsigned int DynamicBuffer::Read(unsigned short* value, unsigned int count)
	{
	return Read<unsigned short>(value,count);
	}

unsigned int DynamicBuffer::Read(unsigned int* value, unsigned int count)
	{
	return Read<unsigned int>(value,count);
	}

unsigned int DynamicBuffer::Read(unsigned long* value, unsigned int count)
	{
	return Read<unsigned long>(value,count);
	}

unsigned int DynamicBuffer::Read(float* value, unsigned int count)
	{
	return Read<float>(value,count);
	}

unsigned int DynamicBuffer::Read(double* value, unsigned int count)
	{
	return Read<double>(value,count);
	}

unsigned int DynamicBuffer::Read(bool* value, unsigned int count)
	{
	return Read<bool>(value,count);
	}


//*** GetPointer ***

void* DynamicBuffer::GetPointer() const
	{
	return buffer_;
	}


//*** Write ***

template <typename TYPE> 
unsigned int DynamicBuffer::Write(const TYPE* value, unsigned int count)
	{
	// Calculate the total number of bytes to be written 	
	unsigned int totalSize=sizeof(TYPE)*count;			

	// Make sure there's room to write all those bytes 	
	Resize(totalSize);										

	// Copy the actual bytes into the buffer 				
	MemCpy(buffer_+position_,value,totalSize);				

	// Update the current position 						
	position_+=totalSize;									

	// Update the current size, if we wrote enough bytes  
	if (size_<position_)									
		{
		size_=position_;									
		}

	// Return the number of values written 				
	return count;											
	}


//*** Read ***

template <typename TYPE> 
unsigned int DynamicBuffer::Read(TYPE* value, unsigned int count)
	{
	// Calculate the total number of bytes to be read 					
	unsigned int totalSize=sizeof(*value)*count;							

	// Make sure we're not trying to read more bytes than there is 		
	if (position_+totalSize>size_)											
		{																	
		count=(size_-position_)/sizeof(*value);								
		totalSize=sizeof(*value)*count;										
		}																	

	// Copy the actual bytes from the buffer 								
	if (buffer_ && totalSize>0)												
		{																	
		MemCpy(value,buffer_+position_,totalSize);							
		}																	

	// Update the current position 										
	position_+=totalSize;													

	// Return the number of elements written 				 				
	return count;															
	}


//*** Resize ***

void DynamicBuffer::Resize(unsigned int size)
	{
	unsigned int newCapacity=capacity_;
	while (newCapacity<(position_+size))
		{
		newCapacity*=2;
		}

	// First-time buffer allocation
	if (!buffer_)
		{
		capacity_=newCapacity;

		// Allocate memory
		buffer_=static_cast<unsigned char*>(Malloc(capacity_));
		Assert(buffer_,"Couldn't allocate memory for dynamic buffer");
		if (!buffer_)
			{
			FatalError("Allocation failed when allocating memory for dynamic buffer");
			}
		return;
		}


	// Check if the buffer needs resizing
	if (newCapacity>capacity_)
		{	
		capacity_=newCapacity;

		// Reallocate the actual buffer
		buffer_=static_cast<unsigned char*>(Realloc(buffer_,capacity_));
		Assert(buffer_,"Couldn't reallocate memory for dynamic buffer");
		if (!buffer_)
			{
			FatalError("Allocation failed when reallocating memory for dynamic buffer");
			}
		}
	}



