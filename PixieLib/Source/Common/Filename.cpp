//*** Filename.cpp ***

#include "Filename.h"
#include "Debug.h"
#include "StandardLibrary.h"


//*** Default Constructor ***

Filename::Filename():
	filenameString_(0)
	{

	}


//*** Constructor (c-style string) ***

Filename::Filename(const char* filename):
	filenameString_(0)
	{
	// Make sure filename conforms to standard format
	Assert(VerifyFilename(filename),
		"Paths and filenames must use forward slash '/' as path separator, must not end with a path separator and must not be of zero length.");
	filenameString_=CorrectFilename(filename);
	}


//*** Constructor (StringId) ***

Filename::Filename(const StringId& filename):
	filenameString_(0)
	{
	// Make sure filename conforms to standard format
	Assert(VerifyFilename(filename.GetString()),
		"Paths and filenames must use forward slash '/' as path separator, must not end with a path separator and must not be of zero length.");
	filenameId_=CorrectFilename(filename);
	}


//*** Copy Constructor ***

Filename::Filename(const Filename& filename):
	filenameString_(0),
	filenameId_(filename.filenameId_)
	{
	if (filename.filenameString_)
		{
		filenameString_=StrDup(filename.filenameString_);
		}
	}


//*** Destructor ***

Filename::~Filename()
	{
	if (filenameString_)
		{
		Free(filenameString_);
		filenameString_=0;
		}
	}


//*** Assignment Operator ***

const Filename& Filename::operator=(const Filename& filename)
	{
	if (filenameString_)
		{
		Free(filenameString_);
		filenameString_=0;
		}

	filenameId_=filename.filenameId_;
	if (filename.filenameString_)
		{
		filenameString_=StrDup(filename.filenameString_);
		}

	return *this;
	}


//*** Comparison Operator (equality) ***

bool Filename::operator==(const Filename& filename) const
	{
	if (filenameString_ || filename.filenameString_)
		{
		if (GetString()==0 || filename.GetString()==0)
			{
			return false;
			}
		return StrICmp(GetString(),filename.GetString())==0;
		}

	return filenameId_==filename.filenameId_;
	}


//*** Comparison Operator (inequality) ***

bool Filename::operator!=(const Filename& filename) const
	{
	return !(*this==filename);
	}


//*** GetString ***

const char* Filename::GetString() const
	{
	if (filenameString_)
		{
		return filenameString_;
		}

	return filenameId_.GetString();
	}


//*** GetStringId ***

StringId Filename::GetStringId() const
	{
	if (filenameString_)
		{
		filenameId_=StringId(filenameString_);
		Free(filenameString_);
		filenameString_=0;
		}

	return filenameId_;
	}


//*** VerifyFilename ***

bool Filename::VerifyFilename(const char* filename)
	{
	// Empty strings are not valid filenames
	if (filename==0)
		{
		return false;
		}

	// Filenames can't have backslash
	const char* backslash=StrChr(filename,'\\');
	if (backslash)
		{
		return false;
		}

	// If it's a path, it may not have a trailing path separator
	if (filename[StrLen(filename)-1]=='/')
		{
		return false;
		}

	// String is a correctly formated filename or path
	return true;
	}


//*** CorrectFilename ***

StringId Filename::CorrectFilename(StringId filename)
	{
	// If it's already a valid filename, just use as is
	if (VerifyFilename(filename.GetString()))
		{
		return filename;
		}

	// Empty string is interpreted as the current path
	if (filename.GetString()==0)
		{
		return StringId(".");
		}

	// Create a correct copy of the string
	char* string=CorrectFilename(filename.GetString());
	StringId returnValue(string);
	Free(string);

	return returnValue;
	}


//*** CorrectFilename ***

char* Filename::CorrectFilename(const char* filename)
	{
	// If it's already a valid filename, just use as is
	if (VerifyFilename(filename))
		{
		return StrDup(filename);
		}

	// Empty string is interpreted as the current path
	if (filename==0 || filename[0]==0)
		{
		return StrDup(".");
		}

	// Create a copy of the string
	char* string=StrDup(filename);
	int length=StrLen(string);

	// Replace all backslash separators with forward slash
	for (int i=0; i<length; i++)
		{
		if (string[i]=='\\')
			{
			string[i]='/';
			}
		}

	// If there's a trailing path separator, remove it
	if (string[length-1]=='/')
		{
		string[length-1]=0;
		}
	
	return string;
	}

