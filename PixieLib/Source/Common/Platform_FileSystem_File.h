/**
 * \class	Platform_FileSystem_File
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of the file system file
 * \author	Mattias Gustavsson	
 * 
 * Handles file manipulation and allows for checking if files exist,
 * read, write and delete. Endian-ness is handled automatically, and
 * the data is always written as little-endian, and converted at read
 * to the native format of the platform.
 */

#ifndef __Platform_FileSystem_File_H__
#define __Platform_FileSystem_File_H__

// Includes

// Forward declares


// Platform_Files_Directory
class Platform_FileSystem_File
	{
	public:
		/**
		 * Destructor
		 */
		virtual ~Platform_FileSystem_File() { };


		/** 
		 * Retrieves the path specified when this file object was created
		 *
		 * \returns	The path for the file object
		 */
		virtual const char* GetPath() = 0;


		/**
		 * Checks if the file which this file object represents exists.
		 *
		 * \returns True if the file exists, false if it doesn't
		 */
		virtual bool Exists() = 0;


		/**
		 * Creates the file which this file object represent. If it already
		 * exists, it will be replaced. The file is then left open for 
		 * writing, and should be closed by a call to Close.
		 *
		 * \returns	True if the file was successfully created, false if there was an error
		 */
		virtual bool Create() = 0;


		/**
		 * Opens the file which this file object represent, leaving it ready for 
		 * reading from. When done, it should be closed by a call to Close.
		 *
		 * \returns	True if the file was successfully opened, false if there was an error
		 */
		virtual bool Open() = 0;

		/**
		 * Close a file which was opened for reading by a call to Open, or for writing
		 * by a call to Create.
		 *
		 * \returns	True if the file was successfully closed, false if there was an error
		 */
		virtual bool Close() = 0;


		/**
		 * Deletes the file which this file object represent, if it exists.
		 */
		virtual void Delete() = 0;


		/** 
		 * Enumeration of possible values to use for the SetPosition method, when specifying
		 * what origin the specified position value is relative to.
		 */
		enum SetPositionOrigin
			{
			ORIGIN_START = 0,		///< Set position relative to the start of the file
			ORIGIN_CURRENT = 1,		///< Set position relative to the current position in the file
			ORIGIN_END = 2,			///< Set position relative to the end of the file
			};


		/**
		 * Sets the read or write position of a file which is currently open for reading or
		 * writing, respectively. The specified position can be relative to the current position,
		 * the start of the file or the end of the file, as specified.
		 *
		 * \returns	The new position
		 */
		virtual int SetPosition(
			int position,	///< The new position
			SetPositionOrigin origin = ORIGIN_START	///< What origin the specified position value is relative to
			) = 0;

		/**
		 * Retrieves the current read or write position of a file which is currently open for reading or
		 * writing, respectively. The returned position is relative to the start of the file.
		 *
		 * \returns	The current position of the file
		 */
		virtual int GetPosition() = 0;

		
		/** 
		 * Writes the specified values to the file. Endianness is handled
		 * automatically by the platform-specific implementation, such 
		 * that everything is written to file as little endian. This is
		 * to ensure binary file compatibility across platforms. 
		 *
		 * \param value Pointer to the value(s) which is to be written
		 * \param count Number of values to write
		 *
		 * \returns	The number of values that was successfully written
		 */
		virtual int Write(const char* value, int count = 1) = 0;
		virtual int Write(const short* value, int count = 1) = 0;
		virtual int Write(const int* value, int count = 1) = 0;
		virtual int Write(const long* value, int count = 1) = 0;
		virtual int Write(const unsigned char* value, int count = 1) = 0;
		virtual int Write(const unsigned short* value, int count = 1) = 0;
		virtual int Write(const unsigned int* value, int count = 1) = 0;
		virtual int Write(const unsigned long* value, int count = 1) = 0;
		virtual int Write(const float* value, int count = 1) = 0;
		virtual int Write(const double* value, int count = 1) = 0;
		virtual int Write(const bool* value, int count = 1) = 0;

		/** 
		 * Reads the specified number of values from the file, into the
		 * specified memory area. Endianness is handled automatically by 
		 * the platform-specific implementation, assuming that values 
		 * stored in the file are all little endian. This is to ensure 
		 * binary file compatibility across platforms. 
		 *
		 * \param value Pointer to the memory where the values are to be read
		 * \param count Number of values to read
		 *
		 * \returns	The number of values that was successfully read
		 */
		virtual int Read(char* value, int count = 1) = 0;
		virtual int Read(short* value, int count = 1) = 0;
		virtual int Read(int* value, int count = 1) = 0;
		virtual int Read(long* value, int count = 1) = 0;
		virtual int Read(unsigned char* value, int count = 1) = 0;
		virtual int Read(unsigned short* value, int count = 1) = 0;
		virtual int Read(unsigned int* value, int count = 1) = 0;
		virtual int Read(unsigned long* value, int count = 1) = 0;
		virtual int Read(float* value, int count = 1) = 0;
		virtual int Read(double* value, int count = 1) = 0;
		virtual int Read(bool* value, int count = 1) = 0;
	};

#endif /* __Platform_FileSystem_File_H__ */
