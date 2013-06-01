//*** StandardLibrary.cpp ****

#include "StandardLibrary.h"
#include "Debug.h"
#include "Random.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

//*** Round ***

int Round(float x)
	{
	#ifdef _WIN32
		int t;

		__asm
			{
			fld  x
			fistp t
			}

		return t;
	#else
		return (int)(x+0.5f);
	#endif
	}


Random standardLibrary_random;

//*** SRand ***

void SRand(unsigned int seed)
	{
	// srand(seed);

	if (seed==1) 
		{
		seed=0x5ee39c34;   // Default seed
		}
	standardLibrary_random.Seed(seed); // Use the WELL algorithm instead of the stdlib one
	}		


//*** Rand ***

unsigned int Rand()
	{
	// return rand();

	return standardLibrary_random.RandomInteger(); // Use the WELL algorithm instead of the stdlib one
	}		


//*** Randomize ***

void Randomize()
	{
//	srand((unsigned int)time(NULL));

	unsigned int seed=(unsigned int)time(NULL); // Use current time as seed
	standardLibrary_random.Seed(seed); // Use the mersienne twister algorithm instead of the stdlib one
	}		


//*** ToUpper *** 

int ToUpper(int c)
	{
	return toupper(c);
	}


//*** ToLower *** 

int ToLower(int c)
	{
	return tolower(c);
	}


//*** StringToInt *** 

int StringToInt(const char* string)
	{
	return atoi(string);
	}


//*** StringToLong *** 

long StringToLong(const char* string)
	{
	return atol(string);
	}


//*** StringToDouble *** 

double StringToDouble(const char* string)
	{
	return atof(string);
	}


//*** StrDup *** 

char* StrDup(const char* string)
	{
	return _strdup(string);
	}


//*** IntToString *** 

char* IntToString(int value, char* string, int maxLength)
	{
	#ifdef WIN32
		return _itoa(value,string,maxLength);
	#else
		return 0;
	#endif
	}


//*** SPrintF *** 

/* We want a link error when SPrintF is used, as it is much safer to use SNPrintF
int SPrintF(char* buffer, const char* string, ...)
	{
	va_list arglist;
	va_start(arglist, string);
	int result=vsprintf(buffer, string, arglist);
	va_end(arglist);
	return result;
	}
*/


//*** SNPrintF *** 

int SNPrintF(char* buffer, int maxLength, const char* string, ...)
	{
	va_list arglist;
	va_start(arglist, string);
	int result=_vsnprintf(buffer, maxLength, string, arglist);
	va_end(arglist);
	return result;
	}


//*** StrNICmp *** 

int StrNICmp(const char* string1, const char* string2, int count)
	{
	return _strnicmp(string1,string2,count);
	}


//*** StrNCmp *** 

int StrNCmp(const char* string1, const char* string2, int count)
	{
	return strncmp(string1,string2,count);
	}


//*** StrICmp *** 

int StrICmp(const char* string1, const char* string2)
	{
	return _stricmp(string1,string2);
	}


//*** StrCmp *** 

int StrCmp(const char* string1, const char* string2)
	{
	return strcmp(string1,string2);
	}


//*** StrLen *** 

unsigned int StrLen(const char* string)
	{
	return (unsigned int)strlen(string);
	}


//*** StrCpy *** 

char* StrCpy(char* strDestination, const char* strSource)
	{
	return strcpy(strDestination,strSource);
	}


//*** StrNCpy *** 

char* StrNCpy(char* strDestination, const char* strSource,int count)
	{
	return strncpy(strDestination,strSource,count);
	}


//*** StrCat *** 

char* StrCat(char* strDestination,  const char* strSource)
	{
	return strcat(strDestination,strSource);
	}


//*** StrNCat *** 

char* StrNCat(char* strDestination,  const char* strSource, int count)
	{
	return strncat(strDestination,strSource,count);
	}


//*** StrUpr *** 

char* StrUpr(char* string)
	{
	return _strupr(string);
	}


//*** StrLwr *** 

char* StrLwr(char* string)
	{
	return _strlwr(string);
	}


//*** StrChr *** 

const char* StrChr(const char* string, int c)
	{
	return strchr(string,c);
	}


//*** StrRChr *** 

const char* StrRChr(const char* string, int c)
	{
	return strrchr(string,c);
	}


//*** StrTok *** 

char* StrTok(char* strToken, const char* strDelimit)
	{
	return strtok(strToken,strDelimit);
	}


//*** StrStr *** 

const char* StrStr(const char* str1, const char* str2)
	{
	return strstr(str1,str2);
	}


//*** Malloc *** 

void* Malloc(unsigned int size)
	{
	void* data=malloc(size);
	Assert(data,"Couldn't allocate memory");
	return data;
	}


//*** Free *** 

void Free(void* memblock)
	{
	free(memblock);
	}	


//*** Realloc *** 

void* Realloc(void* memblock,unsigned int size)
	{
	void* newmemblock=realloc(memblock,size);
	Assert(newmemblock,"Couldn't allocate memory");
	return newmemblock;
	}
	

//*** MemCpy *** 

void* MemCpy(void* dest, const void* src, unsigned int count)
	{
	return memcpy(dest,src,count);
	}
	

//*** MemSet *** 

void* MemSet(void* dest, int c, unsigned int count )
	{
	return memset(dest,c,count);
	}	

	
//*** MemCmp *** 

unsigned int MemCmp(const void* a, const void* b, unsigned int count )
	{
	return memcmp(a,b,count);
	}	


//*** QSort *** 

void QSort(void* base, size_t num, size_t width, int (__cdecl* compare )(const void*, const void*) )
	{
	qsort(base,num,width,compare);
	}


//*** ToRadians ***

float ToRadians(float degrees)
	{	
	return (degrees * PI)/180.0f;
	};


//*** ToDegrees ***

float ToDegrees(float radians)
	{
	return (radians * 180.0f)/PI;
	}
	

//*** AngleDiff ***

float AngleDiff(float angleA, float angleB)
	{
	float angleDiff;

	//Calculate
	angleDiff=angleB-angleA;
		
	// Make sure diff is in the range 0 to TWO_PI
	angleDiff=fmodf(angleDiff,TWO_PI);
	if (angleDiff<0)
		{
		angleDiff+=TWO_PI;
		}

	// Make sure diff is in the range -PI to PI
	if (angleDiff>=PI)
		{
		angleDiff-=TWO_PI;
		}

	// Return the diff
	return angleDiff;
	}
	

//*** Abs ***

float Abs(float x)
	{
	return fabsf(x);
	}
	

//*** Abs ***

int Abs(int x)
	{
	return abs(x);
	}
	

//*** Sqrt ***

float Sqrt(float x)
	{
	return sqrtf(x);
	}
	

//*** Mod ***

float Mod(float x, float y)
	{
	return fmodf(x,y);
	}


//*** Sin ***

float Sin(float x)
	{
	return sinf(x);
	}
	

//*** ASin ***

float ASin(float x)
	{
	return asinf(x);
	}
	

//*** Cos ***

float Cos(float x)
	{
	return cosf(x);
	}


//*** ACos ***

float ACos(float x)
	{
	return acosf(x);
	}


//*** Tan ***

float Tan(float x)
	{
	return tanf(x);
	}


//*** ATan ***

float ATan(float x)
	{
	return atanf(x);
	}


//*** Atan2 ***

float Atan2(float x, float y)
	{
	return atan2f(x,y);
	}	


//*** Pow ***

float Log10(float x)
	{
	return log10f(x);
	}


//*** Pow ***

int Pow(int x, int y)
	{
	return (int)pow((float)x,(int)y);
	}


//*** Pow ***

float Pow(float x, float y)
	{
	return pow(x,y);
	}


//*** Exp ***

float Exp(float x)
	{
	return exp(x);
	}


//*** Log ***

float Log(float x)
	{
	return log(x);
	}


//*** Max ***

float Max(float a, float b)
	{
	if (a>b)
		{
		return a;
		}

	return b;
	}


//*** Min ***

float Min(float a, float b)
	{
	if (a<b)
		{
		return a;
		}

	return b;
	}


//*** Max ***

int Max(int a, int b)
	{
	if (a>b)
		{
		return a;
		}

	return b;
	}


//*** Min ***

int Min(int a, int b)
	{
	if (a<b)
		{
		return a;
		}

	return b;
	}


//*** Clamp ***

int Clamp(int value, int min, int max)
	{
	if (value<min)
		{
		return min;
		}
	if (value>max)
		{
		return max;
		}
	return value;
	}


//*** Clamp ***

float Clamp(float value, float min, float max)
	{
	if (value<min)
		{
		return min;
		}
	if (value>max)
		{
		return max;
		}
	return value;
	}


//*** Floor ***

float Floor(
	float x 
	)
	{
	return floor( x );
	}


//*** Swap ***

void Swap(int& x, int& y)
	{
	int t=x;
	x=y;
	y=t;
	}


//*** Swap ***

void Swap(unsigned int& x, unsigned int& y)
	{
	unsigned int t=x;
	x=y;
	y=t;
	}


//*** Swap ***

void Swap(float& x, float& y)
	{
	float t=x;
	x=y;
	y=t;
	}


//*** GetNearestPowerOfTwo ***

inline unsigned int GetNearestPowerOfTwo(unsigned int v)
	{
	 // compute the next highest power of 2 of 32-bit v
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;	

	return v;
	}


//*** saturate ***

float saturate( float v )
	{
	float t = v < 0.0f ? 0.0f : v;

	return t > 1.0f ? 1.0f : t;
	}


//*** smoothstep ***

float smoothstep( float min, float max, float x )
	{
    // Scale, bias and saturate x to 0..1 range
    x = saturate( ( x - min ) / ( max - min ) ); 
    // Evaluate polynomial
    return x * x * ( 3 - 2 * x );
	}


//*** smootherstep ***

float smootherstep( float min, float max, float x )
	{
    // Scale, and saturate x to 0..1 range
    x = saturate( ( x - min ) / ( max - min ) );
    // Evaluate polynomial
    return x * x * x * ( x * ( x * 6 - 15 ) + 10 );
	}