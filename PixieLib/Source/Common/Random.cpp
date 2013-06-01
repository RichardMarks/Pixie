//*** Random.cpp ***

#include "Random.h"

//*** Constructor ***

Random::Random():
    index_(0)
	{
    Seed(0x5ee39c34);  // Default seed
    }


//*** Seed ***

void Random::Seed(unsigned int s)
	{	
	// Some bit manipulation magic to propagate seed value to the internal state
    state_[0]=s^0xf68a9fc1;
    for (int i=1; i<16; i++) 
		{
        state_[i] = (0x6c078965U * (state_[i-1] ^ (state_[i-1] >> 30)) + i); 
		}
	index_ = 0;
	}


//*** SetState ***

void Random::SetState(const unsigned int state[16], int index)
    {
    for (int i=0; i<16; i++)
        {
        state_[i]=state[i];
        }

    index_=index&15;
    }


//*** GetState ***

void Random::GetState(unsigned int state[16], int& index)
    {
    for (int i=0; i<16; i++)
        {
        state[i]=state_[i];
        }

    index=index_;
    }


//*** RandomInteger ***

unsigned int Random::RandomInteger()
	{
	// This is basically just the WELL algorithm as-is
    unsigned int a = state_[index_];
    unsigned int c = state_[(index_+13)&15];
    unsigned int b = a^c^(a<<16)^(c<<15);
    c = state_[(index_+9)&15];
    c ^= (c>>11);
    a = state_[index_] = b^c;
    unsigned int d = a^((a<<5)&0xda442d24U);
    index_ = (index_ + 15)&15;
    a = state_[index_];
    state_[index_] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return state_[index_];
	}


//*** RandomRanged ***

int Random::RandomRanged(int min, int max)
	{
	// Calculate the range. The +1 is because the RandomFloat method returns
	// values up to, but not including, 1. So, a max/min difference of 0, will 
	// result in a range value of 1, which will (correctly) return all zeros.
	// If we didn't do this, probability that max would come up would be wrong.
	int range=(max-min)+1;

	// If min was greater than max, just return min
	if (range<=0)
		{
		return min;
		}
	
	// Get a random float and scale it with the range. By generating a float and
	// scaling it by range, we guarantee an equal distribution of values, which
	// we are not sure to get if using modulo operator for the range.
	int value=(int)(RandomFloat()*range);
    return min+value; 
	}


//*** RandomFloat ***

float Random::RandomFloat()
	{
    // Get a random integer, and divide by 2^32
    return (RandomInteger()/4294967296.0f);     
	}


