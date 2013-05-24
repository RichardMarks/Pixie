//*** StaticBuffer.cpp ***

#include "StaticBuffer.h"
#include "Debug.h"
#include "StandardLibrary.h"


//*** Constructor ***

StaticBuffer::StaticBuffer(const DynamicBuffer& buffer):
	buffer_(static_cast<unsigned char*>(buffer.GetPointer())),
	size_(buffer.GetSize()),
	position_(0)
	{
	}	


//*** Constructor ***

StaticBuffer::StaticBuffer(const void* buffer, int size):
	buffer_(static_cast<const unsigned char*>(buffer)),
	size_(size),
	position_(0)
	{
	}	


//*** Destructor ***

StaticBuffer::~StaticBuffer()
	{
	}


//*** GetSize ***

int StaticBuffer::GetSize() const
	{
	return size_;
	}


//*** GetPosition ***

int StaticBuffer::GetPosition() const
	{
	return position_;
	}


//*** SetPosition ***

void StaticBuffer::SetPosition(int position) const
	{
	position_=position; 
	}


//*** GetPointer ***

const void* StaticBuffer::GetPointer() const
	{
	return buffer_;
	}


//*** READMACRO ***

// This macro defines the code that is to be executed for all of the overloaded Read methods
// The code is identical for all the different Read methods, only the input parameters change,
// so it makes more sense to use a macro rather than copy-paste. This macro is undefined as
// soon as it is not needed anymore

#define READMACRO()  \
	/* Calculate the total number of bytes to be read */					\
	int totalSize=sizeof(*value)*count;										\
	/* Make sure we're not trying to read more bytes than there is */		\
	if (position_+totalSize>size_)											\
		{																	\
		count=(size_-position_)/sizeof(*value);								\
		totalSize=sizeof(*value)*count;										\
		}																	\
	/* Copy the actual bytes from the buffer */								\
	MemCpy(value,buffer_+position_,totalSize);								\
	/* Update the current position */										\
	position_+=totalSize;													\
	/* Return the number of elements written */				 				\
	return count;															\


//** Read methods */
	
int StaticBuffer::Read(char* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(short* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(int* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(long* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(unsigned char* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(unsigned short* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(unsigned int* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(unsigned long* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(float* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(double* value, int count) const
	{
	READMACRO();
	}

int StaticBuffer::Read(bool* value, int count) const
	{
	READMACRO();
	}

// No need for the read macro anymore, so undefine it
#undef READMACRO
