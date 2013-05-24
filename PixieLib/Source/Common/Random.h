/**
 * \class	Random
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 * Random number generator (Marsienne Twister)
 */

#ifndef __Random_H__
#define __Random_H__

class Random
	{
	public:
		Random();

		/**
		 * Initializes mt_[N] with a seed
		 */
		void Seed(unsigned int seed);

		/**
		 * initialize by an array with array-length
		 * seeds is the array for initializing keys
		 * count is its length
		 * slight change for C++, 2004/2/26
		 */
		void Seed(unsigned int seeds[], int count);

		/** 
		 * Generates a random number on [0,0xffffffff]-interval
		 */
		unsigned int RandomInteger();

		/**
		 * Generates a random number on [0,1]-real-interval
		 */
		float RandomFloat();

		/**
		 * 
		 */
		int RandomRanged(int min, int max);

	private:
		static const int N=624;
		unsigned int mt_[N]; // the array for the state vector
		int mti_;
	};

#endif /* __Random_H__ */
