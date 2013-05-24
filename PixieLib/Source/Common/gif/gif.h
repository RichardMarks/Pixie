//*** gif.h ***

/**
 * This is a heavily modified version of (or rather loosely based on) the Winimage 
 * code by Juan Soulie. The original copyright notice for winimage can be found
 * below.
 *												/Mattias Gustavsson
 */

//  --------------------------------------------------------------------------
//
// Copyright © 2000, Juan Soulie <jsoulie@cplusplus.com>
//
// Permission to use, copy, modify, distribute and sell this software or any
// part thereof and/or its documentation for any purpose is granted without fee
// provided that the above copyright notice and this permission notice appear
// in all copies.
//
// This software is provided "as is" without express or implied warranty of
// any kind. The author shall have no liability with respect to the
// infringement of copyrights or patents that any modification to the content
// of this file or this file itself may incur.
//
// ****************************************************************************



#ifndef __gif_H__
#define __gif_H__

// Includes
#include "Array.h"

// Forward declares
class Asset;

// GifLoader
class GifLoader
	{
	public:

		~GifLoader();

		bool Load(const Asset& asset);

	
	unsigned short width_;
	unsigned short height_;

	int loopCount_;

	struct Cel
		{
		int x;
		int y;
		int delay;
		int width;
		int height;
		int transparency;
		unsigned int palette[256];
		unsigned char* pixels;
		};

	Array<Cel*> cels_;
	};


#endif /* __gif_H__ */
