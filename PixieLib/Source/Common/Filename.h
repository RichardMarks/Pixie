/**
 * \class	Filename
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * \todo	Option to silently correct a filename (no assert for incorrect ones)
 */

#ifndef __Filename_H__
#define __Filename_H__

// Includes
#include "StringId.h"
#include "StringPool.h"

// Forward declares

// Filename
class Filename
	{
	public:
		/**
		 * Default constructor. 
		 */
		Filename();

		/**
		 * Constructor. 
		 */
		Filename(
			const char* filename
			);

		/**
		 * Constructor. 
		 */
		Filename(
			const StringId& filename
			);

		/**
		 * Copy Constructor. 
		 */
		Filename(
			const Filename& filename
			);

		/**
		 * Destructor. 
		 */
		~Filename();

		/**
		 * Assignment operator. 
		 */
		const Filename& operator=(
			const Filename& filename
			);

		/**
		 * Comparison operation (equality). 
		 */
		bool operator==(
			const Filename& filename
			) const;

		/**
		 * Comparison operation (inequality). 
		 */
		bool operator!=(
			const Filename& filename
			) const;


		/**
		 * Used to retrieve the original, c-style string (const char*) for this Filename.
		 *
		 * \returns The zero-terminated string for this filename
		 */
		const char* GetString() const;
	
		StringId GetStringId() const;

	private:
		bool VerifyFilename(const char* filename);
		
		const char* CorrectFilename(const char* filename);
		
		StringId CorrectFilename(StringId filename);


	private:
		mutable const char* filenameString_;	
		mutable StringId filenameId_;
		
		static StringPool stringPool_;
	};


#endif /* __Filename_H__ */
