/**
 * \class	CommandLine
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __CommandLine_H__
#define __CommandLine_H__

// Includes
#include "StringId.h"
#include "Array.h"

// Forward declares

// CommandLine
class CommandLine
	{
	public:
		CommandLine();

		CommandLine(const char* commandLineString);

		/**
		 *
		 */
		bool IsCommandLineFlagSet(
			StringId flag
			);

		/**
		 *
		 */
		int GetCommandLineFlagCount();

		/**
		 *
		 */
		StringId GetCommandLineFlag(
			int index
			);

		/**
		 *
		 */
		int GetCommandLineStringCount();

		/**
		 *
		 */
		StringId GetCommandLineString(
			int index
			);

	private:
		void ParseCommandLine(const char* commandLine);


	private:
		Array<StringId> commandLineFlags_;
		Array<StringId> commandLineStrings_;

	};

#endif /* __CommandLine_H__ */
