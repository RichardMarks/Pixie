/**
 * \class	Singleton
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 *
 */

#ifndef __Singleton_H__
#define __Singleton_H__

// Includes

// Forward declares

template<class TYPE> 
class Singleton
	{
	public:
		Singleton();
		virtual ~Singleton();

		static void CreateInstance();
		static void DestroyInstance();
		static TYPE* GetInstance();
		static bool IsInstanceCreated();

	private:
		static TYPE* staticInstancePointer_;
	};

#include "Singleton.inl"

#endif /* __Singleton_H__ */
