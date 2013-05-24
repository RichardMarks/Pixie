/**
 * \class	Platform_FileSystem_Directory
 *
 * \ingroup	platform
 * \brief	Platform independent abstraction of a file system directory
 * \author	Mattias Gustavsson	
 * 
 * Gives access to a directory and allows for creation and deletion, as 
 * well as enumerating sub-directories and files.
 */

#ifndef __Platform_FileSystem_Directory_H__
#define __Platform_FileSystem_Directory_H__

// Includes

// Forward declares


// Platform_FileSystem_Directory
class Platform_FileSystem_Directory
	{
	public:

		/**
		 * Destructor
		 */
		virtual ~Platform_FileSystem_Directory() { };


		/** 
		 * Retrieves the path specified when this directory object was created
		 *
		 * \returns	The path for the directory object
		 */
		virtual const char* GetPath() = 0;


		/**
		 * Checks if the directory which this directory object represents exists.
		 *
		 * \returns True if the directory exists, false if it doesn't
		 */
		virtual bool Exists() = 0;


		/**
		 * Creates the directory which this directory object represent, if it 
		 * doesn't already exist.
		 */
		virtual void Create() = 0;


		/**
		 * Deletes the directory which this directory object represent, 
		 * if it exists.
		 */
		virtual void Delete() = 0;
		

		/**
		 * When the directory object is first created, it will scan the directory
		 * (if it exists) for sub-directories and files. These lists can be updated
		 * by calling this method, if you have reason to believe that the contents
		 * of the directory might have changed.
		 */
		virtual void Rescan() = 0;


		/**
		 * Retrieves the number of sub-directories in the directory which this 
		 * directory object represents.
		 *
		 * \returns The number of sub-directories to this directory
		 */
		virtual int GetSubdirectoryCount() = 0;


		/**
		 * Retrieves the path to the specified sub-directory.
		 *
		 * \returns The sub-directory at the specified index
		 */
		virtual const char* GetSubdirectory(
			int index	///< Index of the sub-directory to get 
			) = 0;


		/**
		 * Retrieves the number of files in the directory which this 
		 * directory object represents.
		 *
		 * \returns The number of files in this directory
		 */
		virtual int GetFileCount() = 0;


		/**
		 * Retrieves the path of the specified file
		 *
		 * \returns The file at the specified index
		 */
		virtual const char* GetFile(
			int index	///< Index of the sub-file to get
			) = 0;

	};

#endif /* __Platform_FileSystem_Directory_H__ */
