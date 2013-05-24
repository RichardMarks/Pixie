//*** CommandLine.cpp ***

#include "CommandLine.h"
#include "Platform_OS.h"
#include "StandardLibrary.h"

//**** Constructor ***

CommandLine::CommandLine()
	{
	if (Platform::GetPlatform_OS())
		{
		const char* commandLineString=Platform::GetPlatform_OS()->GetCommandLineString();
		if (commandLineString)
			{
			ParseCommandLine(commandLineString);
			}
		}	
	}


//**** Constructor ***

CommandLine::CommandLine(const char* commandLineString)
	{
	if (commandLineString)
		{
		ParseCommandLine(commandLineString);
		}
	}


//*** ParseCommandLine ***

void CommandLine::ParseCommandLine(const char* commandLine)
	{

	while (commandLine && *commandLine)
		{
		if (*commandLine==' ')
			{
			commandLine++;
			}
		else if (*commandLine=='-')
			{
			commandLine++;
			const char* end=StrChr(commandLine,' ');
			if (!end)
				{
				commandLineFlags_.Add(StringId(commandLine));				
				}
			else
				{
				int length=(int)(end-commandLine);
				char* buffer=static_cast<char*>(Malloc(length+1));
				StrNCpy(buffer,commandLine,length);
				buffer[length]='\0';
				commandLineFlags_.Add(StringId(buffer));				
				Free(buffer);
				}
			commandLine=end;
			}
		else if (*commandLine=='"')
			{
			commandLine++;
			const char* end=StrChr(commandLine,'"');
			if (!end)
				{
				commandLineStrings_.Add(StringId(commandLine));				
				commandLine=0;
				}
			else
				{
				int length=(int)(end-commandLine);
				char* buffer=static_cast<char*>(Malloc(length+1));
				StrNCpy(buffer,commandLine,length);
				buffer[length]='\0';
				commandLineStrings_.Add(StringId(buffer));				
				Free(buffer);
				commandLine=end+1;
				}
			}
		else
			{
			const char* end=StrChr(commandLine,' ');
			if (!end)
				{
				commandLineStrings_.Add(StringId(commandLine));				
				}
			else
				{
				int length=(int)(end-commandLine);
				char* buffer=static_cast<char*>(Malloc(length+1));
				StrNCpy(buffer,commandLine,length);
				buffer[length]='\0';
				commandLineStrings_.Add(StringId(buffer));				
				Free(buffer);
				}
			commandLine=end;
			}
		}
	}


//*** IsCommandLineFlagSet ***

bool CommandLine::IsCommandLineFlagSet(StringId flag)
	{
	for (int i=0; i<commandLineFlags_.GetItemCount(); i++)
		{
		if (commandLineFlags_.Get(i)==flag)
			{
			return true;
			}
		}

	return false;	
	}


//*** GetCommandLineFlagCount ***

int CommandLine::GetCommandLineFlagCount()
	{
	return commandLineFlags_.GetItemCount();
	}


//*** GetCommandLineFlag ***

StringId CommandLine::GetCommandLineFlag(int index)
	{
	return commandLineFlags_.Get(index);
	}


//*** GetCommandLineStringCount ***

int CommandLine::GetCommandLineStringCount()
	{
	return commandLineStrings_.GetItemCount();
	}


//*** GetCommandLineString ***

StringId CommandLine::GetCommandLineString(int index)
	{
	return commandLineStrings_.Get(index);
	}


