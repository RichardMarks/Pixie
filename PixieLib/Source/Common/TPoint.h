/**
 * \class	TPoint
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TPoint_H__
#define __TPoint_H__

// Includes

// External classes

// TPoint
struct TPoint
	{
	int x_;
	int y_;

	TPoint():
		x_(0),
		y_(0)
		{

		}

	TPoint(int x, int y):
		x_(x),
		y_(y)
		{

		}
	};

inline TPoint operator + (TPoint a, TPoint b)
	{
	TPoint result;
	result.x_ = a.x_ + b.x_;
	result.y_ = a.y_ + b.y_;
	return result;
	}

#endif /* __TPoint_H__ */
