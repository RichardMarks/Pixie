/**
 * \class	TRect
 * 
 * \ingroup	
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __TRect_H__
#define __TRect_H__

// Includes
#include "TPoint.h"

// External classes

// TRect
struct TRect
	{
	int x_;
	int y_;
	int w_;
	int h_;

	TRect();
	TRect( int x, int y, int w, int h );
	TRect( TPoint p1, TPoint p2 );

	void Move( int dx, int dy );
	void Grow( int dx, int dy );
	TRect Intersect( TRect rect ) const;
	TRect Union( TRect rect ) const;
	bool Contains( TPoint point ) const;
	bool operator == ( TRect rect ) const;
	bool operator != ( TRect rect ) const;
	bool IsEmpty() const;

	};

#endif /* __TRect_H__ */
