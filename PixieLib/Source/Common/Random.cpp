//*** Random.cpp ***

// This is the copyright notice for the original source code for the marsienne twister
// Algorithmically, I'm using the same code, I've just reformatted it and turned it into 
// a c++ class. /Mattias
/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/
#include "Random.h"

//*** Constructor ***

Random::Random()
	{
	mti_=N+1; // mti==N+1 means mt[N] is not initialized
	}


//*** Seed ***

void Random::Seed(unsigned int s)
	{	
    mt_[0]= s & 0xffffffff;
    for (mti_=1; mti_<N; mti_++) 
		{
        mt_[mti_] = (1812433253 * (mt_[mti_-1] ^ (mt_[mti_-1] >> 30)) + mti_); 

		// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
        // In the previous versions, MSBs of the seed affect   
        // only MSBs of the array mt[].                        
        // 2002/01/09 modified by Makoto Matsumoto             
        mt_[mti_] &= 0xffffffff;
        // for >32 bit machines 
		}
	}


//*** Seed ***

void Random::Seed(unsigned int seeds[], int count)
	{
    int i;
	int j;
	int k;
    Seed(19650218);
    i=1; j=0;
    k = (N>count ? N : count);
    for (; k; k--) 
		{
        mt_[i] = (mt_[i] ^ ((mt_[i-1] ^ (mt_[i-1] >> 30)) * 1664525)) + seeds[j] + j; // non linear 
        mt_[i] &= 0xffffffff; // for WORDSIZE > 32 machines 
        i++; j++;
        if (i>=N) 
			{ 
			mt_[0] = mt_[N-1]; i=1; 
			}
        if (j>=count) 
			{
			j=0;
			}
		}
    for (k=N-1; k; k--) 
		{
        mt_[i] = (mt_[i] ^ ((mt_[i-1] ^ (mt_[i-1] >> 30)) * 1566083941)) - i; // non linear
        mt_[i] &= 0xffffffff; // for WORDSIZE > 32 machines
        i++;
        if (i>=N) 
			{ 
			mt_[0] = mt_[N-1]; i=1; 
			}
		}

    mt_[0] = 0x80000000; // MSB is 1; assuring non-zero initial array 
	}


//*** RandomInteger ***

unsigned int Random::RandomInteger()
	{
	const unsigned int M = 397;
	const unsigned int MATRIX_A = 0x9908b0df;   // constant vector a 
	const unsigned int UPPER_MASK = 0x80000000; // most significant w-r bits
	const unsigned int LOWER_MASK = 0x7fffffff; // least significant r bits 
    static unsigned int mag01[2]={0x0, MATRIX_A};

	unsigned long y;
    // mag01[x] = x * MATRIX_A  for x=0,1 

    if (mti_ >= N) 
		{ 
		// generate N words at one time
        int kk;

		// if Seed() has not been called,
		if (mti_ == N+1)
			{			
            Seed(5489); // a default initial seed is used
			}

        for (kk=0;kk<N-M;kk++) 
			{
            y = (mt_[kk]&UPPER_MASK)|(mt_[kk+1]&LOWER_MASK);
            mt_[kk] = mt_[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
			}
        for (;kk<N-1;kk++) 
			{
            y = (mt_[kk]&UPPER_MASK)|(mt_[kk+1]&LOWER_MASK);
            mt_[kk] = mt_[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
			}
        y = (mt_[N-1]&UPPER_MASK)|(mt_[0]&LOWER_MASK);
        mt_[N-1] = mt_[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti_ = 0;
		}
  
    y = mt_[mti_++];

    // Tempering 
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
	}


//*** RandomRanged ***

int Random::RandomRanged(int min, int max)
	{
	int range=(max-min)+1;
	if (range<=0)
		{
		return min;
		}
	int value=RandomInteger()%range;
    return min+value; 
	}


//*** RandomFloat ***

float Random::RandomFloat()
	{
    return (RandomInteger()/4294967296.0f); 
    // divided by 2^32 
	}

