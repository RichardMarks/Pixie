/**
 * \class	Random
 * 
 * \ingroup	core
 * \brief	Random number generator
 * \author	Mattias Gustavsson	
 * 
 * Random number generation, using the WELL algorithm by F. Panneton, P. L'Ecuyer and M. Matsumoto.
 * The creators of the algorithm describes it like this:
 *
 *      Fast uniform random number generators with extremely long periods have been defined and implemented based on 
 *      linear recurrences modulo 2. The twisted GFSR and the Mersenne twister are famous recent examples. Besides the 
 *      period length, the statistical quality of these generators is usually assessed via their equidistribution 
 *      properties. The huge-period generators proposed so far are not quite optimal in that respect. In this paper, 
 *      we propose new generators, with better equidistribution and "bit-mixing" properties for equivalent period length 
 *      and speed. Approximately half of the coefficients of the characteristic polynomial of these generators are 
 *      nonzero. The state of our new generators evolves in a more chaotic way than for the Mersenne twister. We 
 *      illustrate how this can reduce the impact of persistent dependencies among successive output values, which can 
 *      be observed in certain parts of the period of gigantic generators such as the Mersenne twister.
 *
 * More information in the original paper: http://www.iro.umontreal.ca/~panneton/WELLRNG.html
 *
 * This code is originally based on WELL512 C/C++ code written by Chris Lomont (published in Game Programming Gems 7) 
 * and placed in the public domain. There's also an article about it on Lomont's site:
 *
 *      http://lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
 */

#ifndef __Random_H__
#define __Random_H__

class Random
	{
	public:
		/**
		 * Constructor. Initialises generator with a default seed
		 */
		Random();

		/**
		 * Seeds the generator with a custom seed value. 
		 */
		void Seed(
			unsigned int seed	///< Custom seed value
			);

		/**
		 * Retrieves the internal state of the generator. This consists of 16
		 * unsigned ints, and one index value. This state can be stored and
		 * later re-applied to a generator to continue along the same sequence.
		 */
        void GetState(
			unsigned int state[16], ///< Array to receive the internal state from the generator
			int& index ///< Variable to receive the index value from the generator
			);

		/**
		 * Re-applies a state that was previously retrieved by calling GetState.
		 * Useful to continue the generator from a previously saved point.
		 */
        void SetState(
			const unsigned int state[16], ///< Array of state values to be applied to the generator
			int index // Index value to be applied to the generator
			);

		/** 
		 * Generates a random number on [0,0xffffffff]-interval
		 */
		unsigned int RandomInteger();

		/**
		 * Generates a random number on [0,0.99999999999...] interval
		 */
		float RandomFloat();

		/**
		 * Generates a random number on [min,max] interval
		 */
		int RandomRanged(int min, int max);

	private:
        unsigned int state_[16];	///< The internal state of the generator
        unsigned int index_; ///< Current index value

};

#endif /* __Random_H__ */
