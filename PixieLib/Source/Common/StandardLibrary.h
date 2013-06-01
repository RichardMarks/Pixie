/**
 * \file	StandardLibrary.h
 * 
 * \ingroup	core
 * \brief	Standard library functionality.
 * \author	Mattias Gustavsson
 * 
 * Standard library functions. Standard libraries like <stdlib.h> and <math.h> are not to  be included directly, to 
 * avoid compatibility problems when porting to compilers that does not comply to the standard. Instead, this file is 
 * included, and all the things supported by <stdlib.h> and <math.h> can be supported by "StandardLibrary.h" as well 
 * (generally, it only wraps the methods, but on some platforms, custom implementations might be necessary or 
 * desirable).
 */

#ifndef __StandardLibrary_H__
#define __StandardLibrary_H__

/**
 * Rounds the specified float value to the nearest integer value. On some CPU's (such as x86), this is much faster
 * than truncating the value by casting to int.
 *
 * \returns	the nearest integer value
 */
int Round(
	float x
	);


/**
 * The constant RANDOM_MAX is the maximum value that can be returned by the Rand function. 
 */
const unsigned int RANDOM_MAX=0xffffffff;


/** 
 * The Rand function returns a pseudorandom integer in the range 0 to RANDOM_MAX. Use the SRand function to seed the 
 * pseudorandom-number generator before calling Rand.
 *
 * \returns	a pseudorandom number, as described above. There is no error return.
 */
unsigned int Rand();


/**
 * The SRand function sets the starting point for generating a series of pseudorandom integers. To reinitialize the 
 * generator, use 1 as the seed argument. Rand  retrieves the pseudorandom numbers that are generated. Calling Rand 
 * before any call to SRand generates the same sequence as calling SRand with seed passed as 1.
 */
void SRand(
	unsigned int seed	///< Seed for random-number generation 
	);


/**
 * The Randomize function sets the starting point for generating a series of pseudorandom integers to a random starting 
 * point. It is the equivalent of calling SRand passing it the current time as a seed value.
 */
void Randomize();


/**
 * Convert character to uppercase
 */
int ToUpper(
	int c	///< Character to convert. 
	);


/**
 * Convert character to lowercase
 */
int ToLower(
	int c	///< Character to convert. 
	);


/**
 * This function convert a character string to an integer value 
 * The input string is a sequence of characters that can be interpreted as a numerical value of the specified type. The 
 * function stops reading the input string at the first character that it cannot recognize as part of a number. This 
 * character may be the null character ('\0') terminating the string.
 *
 * \returns	the int value produced by interpreting the input characters as a number. The return value is 0 if the input 
 *			cannot be converted to a value of that type. The return value is undefined in case of overflow.
 */
int StringToInt(
	const char* string	///< String to be converted.
	);

/**
 * This function convert a character string to a long integer value 
 * The input string is a sequence of characters that can be interpreted as a numerical value of the specified type. The 
 * function stops reading the input string at the first character that it cannot recognize as part of a number. This 
 * character may be the null character ('\0') terminating the string.
 *
 * The string argument has the form:
 *
 *		[whitespace] [sign]digits
 *
 * A whitespace consists of space or tab characters, which are ignored; sign is either plus (+) or minus ( - ); and 
 * digits are one or more decimal digits.  
 *
 * \returns	the long value produced by interpreting the input characters as a number. The return value is 0L if the  
 *			input cannot be converted to a value of that type. The return value is undefined in case of overflow.
 */
long StringToLong(
	const char* string	///< String to be converted.
	);


/**
 * This function convert a character string to a double precision floating point value 
 * The input string is a sequence of characters that can be interpreted as a numerical value of the specified type. The 
 * function stops reading the input string at the first character that it cannot recognize as part of a number. This 
 * character may be the null character ('\0') terminating the string.
 *
 * The string argument has the following form:
 *
 *		[whitespace] [sign] [digits] [.digits] [ {d | D | e | E }[sign]digits]
 *
 * A whitespace consists of space or tab characters, which are ignored; sign is either plus (+) or minus ( - ); and 
 * digits are one or more decimal digits. If no digits appear before the decimal point, at least one must appear after 
 * the decimal point. The decimal digits may be followed by an exponent, which consists of an introductory letter (d, D,
 * e, or E) and an optionally signed decimal integer.
 *
 * \returns	the double value produced by interpreting the input characters as a number. The return value is 0.0 if the 
 *			input cannot be converted to a value of that type. The return value is undefined in case of overflow.
 */
double StringToDouble(
	const char* string	///< String to be converted.
	);

/**
 * The SPrintF function formats and stores a series of characters and values in buffer. Each argument (if any) is 
 * converted and output according to the corresponding format specification in format. A null character is appended 
 * after the last character written. If copying occurs between strings that overlap, the behavior is undefined.
 *
 *		Security Note   There is no way to limit the number of characters written, which means that code using SPrintF 
 *						is susceptible to buffer overruns. Instead of using SPrintF, you should use the related function
 *						SMprintf, which specifies a maximum number of characters to be written to buffer. 
 *						The implementation of SNPrintF have been commented out in StandardLibrary.h, so you will get a
 *						linker error if you try and use it, as a reminder to use SNPrintF instead.
 *
 * \returns	The number of characters written, or -1 if an error occurred. SPrintF returns the number of bytes stored in 
 *			buffer, not counting the terminating null character. 
 */
int SPrintF(
	char* buffer, ///< Storage location for output 
	const char* string,	///< Format-control string 
	... ///< Optional arguments 
	);


/**
 * The SNPrintF function formats and stores count or fewer characters and values (including a terminating null character 
 * that is always appended unless count is zero or the formatted string length is greater than or equal to count 
 * characters) in buffer. Each argument (if any) is converted and output according to the corresponding format 
 * specification in format. If copying occurs between strings that overlap, the behavior is undefined.
 *
 *		Security Note	Ensure that format is not a user-defined string. This function does not guarantee NULL 
 *						termination, so ensure it is followed by sz[ ARRAYSIZE(sz) - 1] = 0. 
 *
 * 
 *
 * \returns	the number of bytes stored in buffer, not counting the terminating null character. If the number of bytes 
 *			required to store the data exceeds count, then count bytes of data are stored in buffer and a negative value 
 *			is returned. 
 */
int SNPrintF(
	char* buffer,	///< Storage location for output.
	int maxLength,	///< Maximum number of characters to store. 
	const char* string,	///< Format-control string. 
	...	///< Optional arguments.
	);


/**
 * The StrDup function calls Malloc to allocate storage space for a copy of strSource and then copies strSource to the 
 * allocated space. Because StrDup calls Malloc to allocate storage space for the copy of strSource, it is good practice
 * always to release this memory by calling the Free routine on the pointer returned by the call to StrDup.
 *
 * \returns	a pointer to the storage location for the copied string or NULL if storage cannot be allocated.
 */
char* StrDup(
	const char* string	///< Null-terminated source string. 
	);


/**
 * The StrNICmp function lexicographically compares, at most, the first count characters of string1 and string2. The 
 * comparison is performed without regard to case; StrNICmp is a case-insensitive version of StrNCmp. The comparison 
 * ends if a terminating null character is reached in either string before count characters are compared. If the strings 
 * are equal when a terminating null character is reached in either string before count characters are compared, the 
 * shorter string is lesser.
 *
 * Two strings containing characters located between 'Z' and 'a' in the ASCII table ('[', '\', ']', '^', '_', and '`') 
 * compare differently, depending on their case. For example, the two strings "ABCDE" and "ABCD^" compare one way if the 
 * comparison is lowercase ("abcde" > "abcd^") and the other way ("ABCDE" < "ABCD^") if it is uppercase.
 *
 * \returns	< 0 if string1 substring less than string2 substring, 0 if string1 substring identical to string2 substring,
 *			and > 0 if string1 substring greater than string2 substring 
 */
int StrNICmp(
	const char* string1,	///< Null-terminated strings to compare
	const char* string2,	///< Null-terminated strings to compare
	int count	///< Number of characters to compare
	);


/**
 * The StrICmp function lexicographically compares lowercase versions of string1 and string2 and returns a value 
 * indicating their relationship.  Because StrICmp does lowercase comparisons, it may result in unexpected behavior.
 * To illustrate when case conversion by StrICmp affects the outcome of a comparison, assume that you have the two 
 * strings JOHNSTON and JOHN_HENRY. The string JOHN_HENRY will be considered less than JOHNSTON because the "_" has a 
 * lower ASCII value than a lowercase S. In fact, any character that has an ASCII value between 91 and 96 will be 
 * considered less than any letter. If the StrCmp function is used instead of stricmp, JOHN_HENRY will be greater than 
 * JOHNSTON.
 *
 * \returns	< 0 if string1 less than string2,  0 if string1 identical to string2 and > 0 if string1 greater than string2 
 */
int StrICmp(
	const char* string1,	///< Null-terminated strings to compare
	const char* string2		///< Null-terminated strings to compare
	);


/**
 * The StrCmp function compares string1 and string2 lexicographically and returns a value indicating their relationship. 
 *
 * \returns	< 0 if string1 less than string2,  0 if string1 identical to string2 and > 0 if string1 greater than string2 
 */
int StrCmp(
	const char* string1,	///< Null-terminated strings to compare
	const char* string2		///< Null-terminated strings to compare
	);


/**
 * The StrNCmp function lexicographically compares, at most, the first count characters in string1 and string2 and 
 * returns a value indicating the relationship between the substrings. StrNCmp is a case-sensitive version of StrNICmp. 
 *
 * \returns	< 0 if string1 substring less than string2 substring, 0 if string1 substring identical to string2 substring,
 *			and > 0 if string1 substring greater than string2 substring 
 */
int StrNCmp(
	const char* string1,	///< Null-terminated strings to compare
	const char* string2,	///< Null-terminated strings to compare
	int count	///< Number of characters to compare. 

	);


/**
 * Returns the number of characters in string, not including the terminating null character. 
 *
 * Security Note	This function incur a potential threat brought about by a buffer overrun problem. Buffer overrun 
 *					problems are a frequent method of system attack, resulting in an unwarranted elevation of privilege. 
 *
 * \returns	the number of characters in string, excluding the terminal NULL. No return value is reserved to indicate an 
 *			error.
 */
unsigned int StrLen(
	const char* string	///< Null-terminated string. 
	);


/**
 * The StrCpy function copies strSource, including the terminating null character, to the location specified by 
 * strDestination. The behavior of StrCpy is undefined if the source and destination strings overlap.
 *
 *		Security Note	Because StrCpy does not check for sufficient space in strDestination before copying strSource, 
 *						it is a potential cause of buffer overruns. Consider using StrNCpy instead.
 *
 * \returns	the destination string. No return value is reserved to indicate an error.
 */
char* StrCpy(
	char* strDestination,	///< Destination string. 
	const char* strSource	///< Null-terminated source string.
	);


/**
 * The StrNCpy function copies the initial count characters of strSource to strDestination and returns strDestination. 
 * If count is less than or equal to the length of strSource, a null character is not appended automatically to the 
 * copied string. If count is greater than the length of strSource, the destination string is padded with null 
 * characters up to length count. The behavior of StrNCpy is undefined if the source and destination strings overlap.
 *
 *		Security Note	StrNCpy does not check for sufficient space in strDestination; it is therefore a potential cause 
 *						of buffer overruns. Keep in mind that count limits the number of characters copied; it is not a 
 *						limit on the size of strDestination.
 *
 * \returns	the destination string. No return value is reserved to indicate an error.
 */
char* StrNCpy(
	char* strDestination,	///< Destination string.  
	const char* strSource,	///< Null-terminated source string.
	int count	///< Number of characters to be copied. 
	);


/**
 * The strcat function appends strSource to strDestination and terminates the resulting string with a null character. The initial character of strSource overwrites the terminating null character of strDestination. The behavior of strcat is undefined if the source and destination strings overlap.
 *
 *		Security Note   Because StrCat does not check for sufficient space in strDestination before appending strSource,
 *						it is a potential cause of buffer overruns. Consider using StrNCat instead.
 *
 * \returns	the destination string (strDestination). No return value is reserved to indicate an error.
 */
char* StrCat(
	char* strDestination,	///< Null-terminated destination string.
	const char* strSource	///< Null-terminated source string. 
	);


/**
 * The StrNCat function appends, at most, the first count characters of strSource to strDestination. The initial 
 * character of strSource overwrites the terminating null character of strDestination. If a null character appears in 
 * strSource before count characters are appended, StrNCat appends all characters from strSource, up to the null 
 * character. If count is greater than the length of strSource, the length of strSource is used in place of count. The 
 * resulting string is terminated with a null character. If copying takes place between strings that overlap, the 
 * behavior is undefined.
 *
 *		Security Note	StrNCat does not check for sufficient space in strDestination; it is therefore a potential cause 
 *						of buffer overruns. Keep in mind that count limits the number of characters appended; it is not 
 *						a limit on the size of strDestination.
 *
 * \returns	the destination string (strDestination). No return value is reserved to indicate an error.
 */
char* StrNCat(
	char* strDestination,	///< Null-terminated destination string.
	const char* strSource,	///< Null-terminated source string. 
	int count	///< Number of characters to append. 
	);


/**
 * The StrUpr function converts, in place, each lowercase letter in string to uppercase
 *
 * \returns	a pointer to the altered string. Because the modification is done in place, the pointer returned is the same 
 *			as the pointer passed as the input argument. No return value is reserved to indicate an error.
 */
char* StrUpr(
	char* string	///< Null-terminated string to capitalize. 
	);


/**
 * The StrLwr function converts, in place, each uppercase letter in string to lowercase
 *
 * \returns	a pointer to the altered string. Because the modification is done in place, the pointer returned is the same 
 *			as the pointer passed as the input argument. No return value is reserved to indicate an error
 */
char* StrLwr(
	char* string	///< Null-terminated string to convert to lowercase. 
	);


/**
 * The StrChr function finds the first occurrence of c in string, or it returns NULL if c is not found. The null-
 * terminating character is included in the search.
 *
 * \returns	A pointer to the first occurrence of c in string, or NULL if c is not found.
 */
const char* StrChr(
	const char* string,	///< Null-terminated string to search.
	int c	///< Character to be located. 
	);


/**
 * The StrRChr function finds the last occurrence of c (converted to char) in string. The search includes the 
 * terminating null character.
 *
 * \returns	a pointer to the last occurrence of c in string, or NULL if c is not found.
 */
const char* StrRChr(
	const char* string,	///< Null-terminated string to search.
	int c	///< Character to be located. 
	);


/**
 * The StrTok function finds the next token in strToken. The set of characters in strDelimit specifies possible 
 * delimiters of the token to be found in strToken on the current call. 
 *
 * On the first call to StrTok, the function skips leading delimiters and returns a pointer to the first token in 
 * strToken, terminating the token with a null character. More tokens can be broken out of the remainder of strToken by 
 * a series of calls to StrTok. Each call to StrTok modifies strToken by inserting a null character after the token 
 * returned by that call. To read the next token from strToken, call strtok with a NULL value for the strToken argument. 
 * The NULL strToken argument causes StrTok to search for the next token in the modified strToken. The strDelimit 
 * argument can take any value from one call to the next so that the set of delimiters may vary.
 *
 *		Note	This function uses a static variable for parsing the string into tokens. If multiple or simultaneous 
 *				calls are made to StrTok, a high potential for data corruption and inaccurate results exists. Therefore, 
 *				do not attempt to call StrTok simultaneously for different strings and be aware of calling this function 
 *				from within a loop where another routine may be called that uses the same function. However, calling 
 *				this function simultaneously from multiple threads does not have undesirable effects.
 *
 * \returns	a pointer to the next token found in strToken. Returns NULL when no more tokens are found. Each call 
 *			modifies strToken by substituting a NULL character for each delimiter that is encountered.
 */
char* StrTok(
	char* strToken,	///< String containing token or tokens. 
	const char* strDelimit	///< Set of delimiter characters. 
	);


/**
 * The strstr function returns a pointer to the first occurrence of strSearch in string. The search does not include 
 * terminating null characters.
 *
 * \returns	a pointer to the first occurrence of strSearch in string, or NULL if strSearch does not appear in string. 
 *			If strSearch points to a string of zero length, the function returns string.
 */
const char* StrStr(
	const char* str1,	///< Null-terminated string to search. 
	const char* str2	///< Null-terminated string to search for. 
	);


/**
 * The Malloc function allocates a memory block of at least size bytes. The block may be larger than size bytes because 
 * of space required for alignment and maintenance information.
 *
 * \returns	A void pointer to the allocated space, or NULL if there is insufficient memory available. To return a 
 *			pointer to a type other than void, use a type cast on the return value. The storage space pointed to by the 
 *			return value is guaranteed to be suitably aligned for storage of any type of object. If size is 0, Malloc 
 *			allocates a zero-length item in the heap and returns a valid pointer to that item. Always check the return 
 *			from Malloc, even if the amount of memory requested is small.
 */
void* Malloc(
	unsigned int size	///< Bytes to allocate. 
	); 


/**
 *`The free function deallocates a memory block (memblock) that was previously allocated by a call to Malloc or Realloc. 
 * The number of freed bytes is equivalent to the number of bytes requested when the block was allocated (or 
 * reallocated, in the case of realloc). If memblock is NULL, the pointer is ignored and free immediately returns. 
 * Attempting to free an invalid pointer (a pointer to a memory block that was not allocated by Malloc or Realloc) may 
 * affect subsequent allocation requests and cause errors.
 */
void Free(
	void* memblock	///< Previously allocated memory block to be freed. 
	);


/**
 * The Realloc function changes the size of an allocated memory block. The memblock argument points to the beginning of 
 * the memory block. If memblock is NULL, Realloc behaves the same way as Malloc and allocates a new block of size 
 * bytes. If memblock is not NULL, it should be a pointer returned by a previous call to malloc or realloc.
 *
 * The size argument gives the new size of the block, in bytes. The contents of the block are unchanged up to the 
 * shorter of the new and old sizes, although the new block can be in a different location. Because the new block can be 
 * in a new memory location, the pointer returned by Realloc is not guaranteed to be the pointer passed through the 
 * memblock argument.
 *
 * \returns	A void pointer to the reallocated (and possibly moved) memory block. The return value is NULL if the size is 
 *			zero and the buffer argument is not NULL, or if there is not enough available memory to expand the block to 
 *			the given size. In the first case, the original block is freed. In the second, the original block is 
 *			unchanged. The return value points to a storage space that is guaranteed to be suitably aligned for storage 
 *			of any type of object. To get a pointer to a type other than void, use a type cast on the return value.
 */
void* Realloc(
	void* memblock,	///< Pointer to previously allocated memory block. 
	unsigned int size	///< New size in bytes. 
	);


/**
 * Copies count bytes of src to dest. If the source and destination overlap, the behavior of memcpy is undefined. 
 * 
 *		Security Note	Make sure that the destination buffer is the same size or larger than the source buffer.
 *
 * \returns	The value of dest.
 */
void* MemCpy(
	void* dest,	///< New buffer.
	const void* src,	///< Buffer to copy from.
	unsigned int count	///< Number of bytes to copy.
	);


/**
 * Sets the first count chars of dest to the character c.
 *
 *		Security Note	Make sure that the destination buffer is the same size or larger than the source buffer. 
 *
 * \returns	The value of dest.
 */
void* MemSet(
	void* dest,	///< Pointer to destination.
	int c,	///< Character to set. 
	unsigned int count	///< Number of characters. 
	);


/**
 * Compares the first count bytes of buf1 and buf2 and returns a value indicating their relationship.
 * 
 * \returns	< 0 if buf1 less than buf2, 0 if buf1 identical to buf2 and > 0 if buf1 greater than buf2 
 */
unsigned int MemCmp(
	const void* buf1,	///< First buffer. 
	const void* buf2,	///< Second buffer. 
	unsigned int count	///< Number of bytes. 
	);


/**
 * The QSort function implements a quick-sort algorithm to sort an array of num elements, each of width bytes. The 
 * argument base is a pointer to the base of the array to be sorted. QSort overwrites this array with the sorted 
 * elements. The argument compare is a pointer to a user-supplied routine that compares two array elements and returns a 
 * value specifying their relationship. QSort calls the compare routine one or more times during the sort, passing 
 * pointers to two array elements on each call:
 *	
 *		compare( (void *) elem1, (void *) elem2 );
 * 
 * The routine must compare the elements, then return < 0 if elem1 less than elem2, 0 if elem1 equivalent to elem2, and
 * > 0 if elem1 greater than elem2 
 *
 * The array is sorted in increasing order, as defined by the comparison function. To sort an array in decreasing order, 
 * reverse the sense of "greater than" and "less than" in the comparison function.
 */
void QSort(
	void* base,	///< Start of target array. 
	size_t num,	///< Array size in elements. 
	size_t width,	///< Element size in bytes. 
	int (__cdecl* compare )(const void*, const void*)	///< Comparison function. The first parameter is a pointer to 
														///< the key for the search and the second parameter is a 
														///< pointer to the array element to be compared with the key. 
	);


/**
 * PI, the ratio of the circumference of a circle to its diameter (with more precision than you'll ever need)
 */
const float PI = 3.1415926535897932384626433832795f;


/**
 * Definition of two times PI (purely for conveniency)
 */
const float TWO_PI = 2*PI;


/**
 * max value
 */
const float FLOAT_MAX = 3.402823466e+38f;	


/** 
 * min positive value
 */
const float FLOAT_MIN = 1.175494351e-38f;	


/** 
 * A really small floating point value. Useful when, for example, comparing if two floats are equal. It is the smallest
 * value such that 1.0+FLOAT_EPSILON != 1.0 
 */
const float FLOAT_EPSILON = 1.192092896e-07f; 


/** 
 * minimum (signed) int value 
 */
const int MIN_INT = (-2147483647 - 1);


/**
 * maximum (signed) int value 
 */
const int MAX_INT = 2147483647;


/**
 * Converts an angle from degrees to radians
 *
 * \returns The angle in radians
 */
float ToRadians(
	float degrees	// Angle to convert, in degrees
	);


/**
 * Converts an angle from radians to degrees
 *
 * \returns The angle in degrees
 */
float ToDegrees(
	float radians	// Angle to convert, in radians
	);


/**
 * Calculates the difference between two angles. The result is quaranteed to be between -PI and PI, regardless of what 
 * angleA and angleB might be
 *
 * \returns	The difference between the two angles
 */ 
float AngleDiff(
	float angleA,	// First angle, in radians
	float angleB	// Second angle, in radians
	);


/**
 * Calculates the absolute value of the floating-point argument.
 *
 * \returns The absolute value of its argument. There is no error return.
 */
float Abs(
	float x	///< Floating-point value. 
	);	


/**
 * Calculates the absolute value.
 *
 * \returns The absolute value of its parameter. There is no error return.
 */
int Abs(
	int x	///< Integer value. 
	);	


/**
 * The Pow function computes x raised to the power of y. 
 *
 * \returns  The value of x raised to the power of y.
 */
int Pow(
	int x,	///< Base. 
	int y	///< Exponent. 
	);	


/**
 * The Pow function computes x raised to the power of y. 
 *
 * \returns  The value of x raised to the power of y.
 */
float Pow(
	float x,	///< Base. 
	float y		///< Exponent. 
	);	


/**
 * Calculates the base-10 logarithm.
 *
 * \returns The Log10 function return the base-10 logarithm of x if successful. If x is negative, the functions return 
 *			an indefinite, by default. If x is 0, it return INF (infinite).
 */
float Log10(
	float x	///< Value whose logarithm is to be found. 
	);


/**
 * Calculates the square root of x. If x is negative, it returns an indefinite.
 *
 * \returns The square-root of x. 
 */
float Sqrt(
	float x	///< Nonnegative floating-point value 
	);


/**
 * Calculates the floating-point remainder of x / y. If the value of y is 0.0,  it returns a quiet NaN.
 *
 * \returns The floating-point remainder.
 */
float Mod(
	float x, ///< Floating-point value. 
	float y	///< Floating-point value. 
	);


/**
 * Calculate sine of x. If x is greater than or equal to 263, or less than or equal to -263, a loss of significance in 
 * the result occurs.
 *
 * \returns The sine of x. 
 */
float Sin(
	float x	///< Value whose sine is to be calculated
	);


/**
 * The ASin function returns the arcsine of x in the range -PI/2 to PI/2 radians. If x is less than -1 or greater than 
 * 1, ASin returns an indefinite by default.
 *
 * \returns The arcsine of x
 */
float ASin(
	float x	///< Value between -1 and 1 whose arcsine is to be calculated. 
	);


/**
 * Calculate cosine of x. If x is greater than or equal to 263, or less than or equal to -263, a loss of significance in 
 * the result occurs.
 *
 * \returns The cosine of x. 
 */
float Cos(
	float x	///< Value whose cosine is to be calculated
	);


/**
 * The ACos function returns the arccosine of x in the range 0 to PI radians. If x is less than -1 or greater than 1, 
 * ACos returns an indefinite by default.
 *
 * \returns The arccosine of x
 */
float ACos(
	float x	///< Value between -1 and 1 whose arccosine is to be calculated. 
	);


/**
 * Tan returns the tangent of x. If x is greater than or equal to 263, or less than or equal to -263, a loss of 
 * significance in the result occurs.
 *
 * \returns	The tangent of x
 */
float Tan(
	float x	///< Angle in radians. 
	);


/**
 * ATan returns the arctangent of x in the range of -PI/2 to PI/2 radians. If x is 0, ATan returns 0.
 * 
 * \returns	The arctangent of x
 */
float ATan(
	float x	///< Value whose arctangent is to be calculated
	);

/**
 * Calculates the arctangent of y/x in the range -PI to PI radians. If x is 0, Atan2 
 * returns 0. If both parameters of Atan2 are 0, the function returns 0. 
 * Atan2 uses the signs of both parameters to determine the quadrant of the return value. 
 * Atan2 is well defined for every point other than the origin, even if x equals 0 and y 
 * does not equal 0.
 *
 * \returns the arctangent of y/x 
 */
float Atan2(
	float x,	///< Any number	
	float y		///< Any number
	);


/**
 * The Exp function returns the exponential value of the floating-point parameter, x, if successful. On overflow, the 
 * function returns INF (infinite) and on underflow, Exp returns 0.
 *
 * \returns	The exponential of x
 */
float Exp(
	float x	///< Floating-point value. 
	);


/**
 * The Log function return the natural logarithm of x if successful. If x is negative, the function return an 
 * indefinite, by default. If x is 0, it return INF (infinite).
 *
 * \returns	The natural logarithm of x
 */
float Log(
	float x	///< Value whose logarithm is to be found. 
	);


/**
 * Returns the larger of two values.
 *
 * \returns	The larger of its arguments.
 */
float Max(
	float a, ///< First of the values to be compared
	float b ///< Second of the values to be compared
	);


/**
 * Returns the smaller of two values.
 *
 * \returns	The smaller of its arguments.
 */
float Min(
	float a, ///< First of the values to be compared
	float b ///< Second of the values to be compared
	);


/**
 * Returns the larger of two values.
 *
 * \returns	The larger of its arguments.
 */
int Max(
	int a, ///< First of the values to be compared
	int b ///< Second of the values to be compared
	);


/**
 * Returns the smaller of two values.
 *
 * \returns	The smaller of its arguments.
 */
int Min(
	int a, ///< First of the values to be compared
	int b ///< Second of the values to be compared
	);


/**
 * Clamps value to the range [min, max].
 * 
 * \returns	If value is between min and max, it is returned as is. If it is smaller than min, min is returned. If it is
 *			larger than max, max is returned.
 */
int Clamp(
	int value,	///< Value to clamp
	int min,	///< Min allowed value
	int max		///< Max allowed value
	);


/**
 * Clamps value to the range [min, max].
 * 
 * \returns	If value is between min and max, it is returned as is. If it is smaller than min, min is returned. If it is
 *			larger than max, max is returned.
 */
float Clamp(
	float value,	///< Value to clamp 
	float min,		///< Min allowed value 
	float max		///< Max allowed value
	);


float Floor (
	float x 
	);

/**
 * Exchanges the value of variables, assigning the contents of the first variable to the second variable, and the 
 * contents of the second to the first.
 */
void Swap(
	int& x,	///< First variable
	int& y	///< Second variable
	);


/**
 * Exchanges the value of variables, assigning the contents of the first variable to the second variable, and the 
 * contents of the second to the first.
 */
void Swap(
	unsigned int& x,	///< First variable 
	unsigned int& y		///< Second variable
	);


/**
 * Exchanges the value of variables, assigning the contents of the first variable to the second variable, and the 
 * contents of the second to the first.
 */
void Swap(
	float& x,	///< First variable 
	float& y	///< Second variable
	);


unsigned int GetNearestPowerOfTwo(unsigned int v);

float saturate( float v );
float smoothstep( float min, float max, float x );
float smootherstep( float min, float max, float x );

#endif /* __StandardLibrary_H__ */
