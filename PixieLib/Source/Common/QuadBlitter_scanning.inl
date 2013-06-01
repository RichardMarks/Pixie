#define SETUP_SECOND_UV
#ifdef USE_UV
	#undef SETUP_SECOND_UV 
	#define SETUP_SECOND_UV u0=u0b;v0=v0b;u1=u1b;v1=v1b;
#endif

#define SET_UV(uv0u,uv0v,uv1u,uv1v) u0=uv0u;v0=uv0v;u1=uv1u;v1=uv1v;


#define SET_UV_TWO(uv0u,uv0v,uv1u,uv1v,uv0ub,uv0vb,uv1ub,uv1vb) u0=uv0u;v0=uv0v;u1=uv1u;v1=uv1v; u0b=uv0ub;v0b=uv0vb;u1b=uv1ub;v1b=uv1vb;

#define INT_SETUP_UV
#ifdef USE_UV
	#undef INT_SETUP_UV
	#define INT_SETUP_UV \
		int stepU=(u1-u0)/(DeltaY);\
		int stepV=(v1-v0)/(DeltaY);\

#endif

#define INT_UPDATE_UV
#ifdef USE_UV
	#undef INT_UPDATE_UV
	#define INT_UPDATE_UV \
		u0+=stepU;\
		v0+=stepV;\

#endif

#define INT_ASSIGN_UV
#ifdef USE_UV
	#undef INT_ASSIGN_UV
	#define INT_ASSIGN_UV \
		(*tempBuffer).u=u0; \
		(*tempBuffer).v=v0; \

#endif

#define INTERPOLATE_EDGE_XMAJOR(X0,Y0,DeltaX,DeltaY,XDirection)\
	{\
	INT_SETUP_UV\
	int XMajorAdvanceAmt=(DeltaX/DeltaY)*XDirection;\
	int ErrorTermAdvance=DeltaX%DeltaY;\
	int ErrorTerm=0;\
	int height=DeltaY;\
	if (XDirection<0)\
		{\
		ErrorTerm = -DeltaY + 1;   \
		}\
	(*tempBuffer).x=(int)X0; \
	INT_ASSIGN_UV\
	while ( DeltaY-- )\
		{\
		INT_UPDATE_UV\
		X0 += XMajorAdvanceAmt;    \
		ErrorTerm += ErrorTermAdvance;\
		if (ErrorTerm > 0) \
			{\
			X0 += XDirection;	\
			ErrorTerm -= height; \
			}\
		tempBuffer+=2;\
		(*tempBuffer).x=(int)X0; \
		INT_ASSIGN_UV\
		}\
	}\



#define INTERPOLATE_EDGE_YMAJOR(X0,Y0,DeltaX,DeltaY,XDirection)\
	{\
	INT_SETUP_UV\
	/* Set up initial error term and values used inside drawing loop */\
	int DeltaXx2 = DeltaX + DeltaX;\
	int DeltaXx2MinusDeltaYx2 = DeltaXx2 - ( DeltaY + DeltaY );\
	int ErrorTerm = DeltaXx2 - DeltaY;\
	(*tempBuffer).x=(int)X0; \
	INT_ASSIGN_UV\
	while ( DeltaY-- ) \
		{\
		INT_UPDATE_UV\
		/* See if it's time to advance the X coordinate */\
		if ( ErrorTerm >= 0 ) \
			{\
			/* Advance the X coordinate & adjust the error term	back down */\
			X0 += XDirection;\
			ErrorTerm += DeltaXx2MinusDeltaYx2;\
			} \
		else \
			{\
			/* Add to the error term */\
			ErrorTerm += DeltaXx2;\
			}\
		tempBuffer+=2;\
		(*tempBuffer).x=(int)X0; \
		INT_ASSIGN_UV\
		}\
	}\



#define EGDE_INTERPOLATION(XP0,YP0,XP1,YP1) \
	{\
	int X0=XP0;\
	int Y0=YP0;\
	int X1=XP1;\
	int Y1=YP1;\
	/* calculate the length of the line in each coordinate */\
	int DeltaX=X1-X0;    \
	int DeltaY=Y1-Y0;\
	/* Handle as four separate cases, for the four octants in which Y1 is greater than Y0 */\
	if (DeltaX>0 ) \
		{\
		int XDirection=1;\
		if (DeltaX>DeltaY)\
			{\
			INTERPOLATE_EDGE_XMAJOR(X0, Y0, DeltaX, DeltaY, 1);\
			} \
		else \
			{\
			INTERPOLATE_EDGE_YMAJOR(X0, Y0, DeltaX, DeltaY, 1);\
			}\
		} \
	else \
		{\
		int XDirection=-1;\
		DeltaX=-DeltaX; /* absolute value of DeltaX */\
		if (DeltaX>DeltaY ) \
			{\
			INTERPOLATE_EDGE_XMAJOR(X0, Y0, DeltaX, DeltaY, -1);\
			} \
		else \
			{\
			INTERPOLATE_EDGE_YMAJOR(X0, Y0, DeltaX, DeltaY, -1);\
			}\
		}\
	}\


#define ScanEdge(x1, y1, x2, y2, edgeBufferPtr) \
	{ \
	int tempX1=x1;\
	int tempY1=y1;\
	int tempX2=x2;\
	int tempY2=y2;\
	EDGE_TYPE* tempBuffer=edgeBufferPtr;\
	EGDE_INTERPOLATION(tempX1,tempY1,tempX2,tempY2);	\
	}\


#define ScanTwoEdges(x1a, y1a, x2a, y2a, x1b, y1b, x2b, y2b, edgeBufferPtr) \
	{ \
	int tempX1a=x1a;\
	int tempY1a=y1a;\
	int tempX2a=x2a;\
	int tempY2a=y2a;\
	int tempX1b=x1b;\
	int tempY1b=y1b;\
	int tempX2b=x2b;\
	int tempY2b=y2b;\
	EDGE_TYPE* tempBuffer=edgeBufferPtr;\
	EGDE_INTERPOLATION(tempX1a,tempY1a,tempX2a,tempY2a);	\
	SETUP_SECOND_UV\
	EGDE_INTERPOLATION(tempX1b,tempY1b,tempX2b,tempY2b);	\
	}\


//*** MAIN CODE ***


	int p0x=targetX1;
	int p0y=targetY1;
	int p1x=targetX2;
	int p1y=targetY2;
	int p2x=targetX3;
	int p2y=targetY3;
	int p3x=targetX4;
	int p3y=targetY4;

	#ifdef USE_UV
		int s0x=sourceX1<<16;
		int s0y=sourceY1<<16;
		int s1x=sourceX2<<16;
		int s1y=sourceY2<<16;
		int s2x=sourceX3<<16;
		int s2y=sourceY3<<16;
		int s3x=sourceX4<<16;
		int s3y=sourceY4<<16;
	#endif

	// order by y
	if (p1y<p0y)
		{
		Swap(p1x,p0x);
		Swap(p1y,p0y);
		#ifdef USE_UV
			Swap(s1x,s0x);
			Swap(s1y,s0y);
		#endif
		}
	if (p2y<p0y)
		{
		Swap(p2x,p0x);
		Swap(p2y,p0y);
		#ifdef USE_UV
			Swap(s2x,s0x);
			Swap(s2y,s0y);
		#endif
		}
	if (p3y<p0y)
		{
		Swap(p3x,p0x);
		Swap(p3y,p0y);
		#ifdef USE_UV
			Swap(s3x,s0x);
			Swap(s3y,s0y);
		#endif
		}
	if (p2y<p1y)
		{
		Swap(p2x,p1x);
		Swap(p2y,p1y);
		#ifdef USE_UV
			Swap(s2x,s1x);
			Swap(s2y,s1y);
		#endif
		}
	if (p3y<p1y)
		{
		Swap(p3x,p1x);
		Swap(p3y,p1y);
		#ifdef USE_UV
			Swap(s3x,s1x);
			Swap(s3y,s1y);
		#endif
		}
	if (p3y<p2y)
		{
		Swap(p3x,p2x);
		Swap(p3y,p2y);
		#ifdef USE_UV
			Swap(s3x,s2x);
			Swap(s3y,s2y);
		#endif
		}

	Assert(p0y<=p1y && p1y<=p2y && p2y<=p3y,"Sort order error");

	// Allocate or reallocate edge buffer if needed
	edgeBuffer_.SetMinSize(sizeof(EDGE_TYPE)*2*(p3y-p0y+1));
	EDGE_TYPE* edgeBuffer=static_cast<EDGE_TYPE*>(edgeBuffer_.buffer_);

	#ifdef USE_UV
		int u0=0;
		int v0=0;
		int u1=0;
		int v1=0;
		int u0b=0;
		int v0b=0;
		int u1b=0;
		int v1b=0;
	#endif

	// Flat top and bottom?
	if (p0y==p1y && p2y==p3y)
		{
		if (p0x<p1x)
			{
			if (p2x<p3x)
				{
				Swap(p3x,p2x);
				Swap(p3y,p2y);
				#ifdef USE_UV
					Swap(s3x,s2x);
					Swap(s3y,s2y);
				#endif
				}
			#ifdef USE_UV
				SET_UV(s0x,s0y,s3x,s3y);
			#endif
			ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer);	

			#ifdef USE_UV
				SET_UV(s1x,s1y,s2x,s2y);
			#endif
			ScanEdge(p1x,p1y,p2x,p2y,edgeBuffer+1);	
			}
		else
			{
			if (p2x>p3x)
				{
				Swap(p3x,p2x);
				Swap(p3y,p2y);
				#ifdef USE_UV
					Swap(s3x,s2x);
					Swap(s3y,s2y);
				#endif
				}
			#ifdef USE_UV
				SET_UV(s1x,s1y,s2x,s2y);
			#endif
			ScanEdge(p1x,p1y,p2x,p2y,edgeBuffer);	

			#ifdef USE_UV
				SET_UV(s0x,s0y,s3x,s3y);
			#endif
			ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer+1);	
			}
		}
	
	// Flat top?
	else if (p0y==p1y)
		{
		if (p0x<p1x)
			{
			if (p2x<p3x)
				{
				//0 to 2, 2 to 3
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s2x,s2y,s2x,s2y,s3x,s3y);
				#endif
				ScanTwoEdges(p0x,p0y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer);

				//1 to 3
				#ifdef USE_UV
					SET_UV(s1x,s1y,s3x,s3y);
				#endif
				ScanEdge(p1x,p1y,p3x,p3y,edgeBuffer+1);					
				}
			else
				{
				//0 to 3
				#ifdef USE_UV
					SET_UV(s0x,s0y,s3x,s3y);
				#endif
				ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer);					

				//1 to 2, 2 to 3
				#ifdef USE_UV
					SET_UV_TWO(s1x,s1y,s2x,s2y,s2x,s2y,s3x,s3y);
				#endif
				ScanTwoEdges(p1x,p1y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer+1);
				}
			}
		else
			{
			if (p2x<p3x)
				{
				// 1 to 2, 2 to 3
				#ifdef USE_UV
					SET_UV_TWO(s1x,s1y,s2x,s2y,s2x,s2y,s3x,s3y);
				#endif
				ScanTwoEdges(p1x,p1y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer);

				//0 to 3
				#ifdef USE_UV
					SET_UV(s0x,s0y,s3x,s3y);
				#endif
				ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer+1);					
				}
			else
				{
				// 1 to 3
				#ifdef USE_UV
					SET_UV(s1x,s1y,s3x,s3y);
				#endif
				ScanEdge(p1x,p1y,p3x,p3y,edgeBuffer);					

				// 0 to 2, 2 to 3
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s2x,s2y,s2x,s2y,s3x,s3y);
				#endif
				ScanTwoEdges(p0x,p0y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer+1);
				}
			}
		}
	
	// Flat bottom?
	else if (p2y==p3y)
		{
		if (p3x<p2x)
			{
			if (p1x<p0x)
				{
				// 0 to 1, 1 to 3
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s3x,s3y);
				#endif
				ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p3x,p3y,edgeBuffer);

				// 0 to 2
				#ifdef USE_UV
					SET_UV(s0x,s0y,s2x,s2y);
				#endif
				ScanEdge(p0x,p0y,p2x,p2y,edgeBuffer+1);					
				}
			else
				{
				// 0 to 3
				#ifdef USE_UV
					SET_UV(s0x,s0y,s3x,s3y);
				#endif
				ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer);					

				// 0 to 1, 1 to 2
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s2x,s2y);
				#endif
				ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p2x,p2y,edgeBuffer+1);
				}
			}
		else
			{
			if (p0x<p1x)
				{
				// 0 to 2
				#ifdef USE_UV
					SET_UV(s0x,s0y,s2x,s2y);
				#endif
				ScanEdge(p0x,p0y,p2x,p2y,edgeBuffer);					

				// 0  to 1, 1 to 3
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s3x,s3y);
				#endif
				ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p3x,p3y,edgeBuffer+1);
				}
			else
				{
				// 0 to 1, 1 to 2
				#ifdef USE_UV
					SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s2x,s2y);
				#endif
				ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p2x,p2y,edgeBuffer);

				// 0 to 3
				#ifdef USE_UV
					SET_UV(s0x,s0y,s3x,s3y);
				#endif
				ScanEdge(p0x,p0y,p3x,p3y,edgeBuffer+1);					
				}

			}
		}

	// Generic case
	else
		{
		if (p1x<p2x)
			{
			#ifdef USE_UV
				SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s3x,s3y);
			#endif
			ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p3x,p3y,edgeBuffer);

			#ifdef USE_UV
				SET_UV_TWO(s0x,s0y,s2x,s2y,s2x,s2y,s3x,s3y);
			#endif
			ScanTwoEdges(p0x,p0y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer+1);
			}
		else
			{
			#ifdef USE_UV
				SET_UV_TWO(s0x,s0y,s2x,s2y,s2x,s2y,s3x,s3y);
			#endif
			ScanTwoEdges(p0x,p0y,p2x,p2y,p2x,p2y,p3x,p3y,edgeBuffer);

			#ifdef USE_UV
				SET_UV_TWO(s0x,s0y,s1x,s1y,s1x,s1y,s3x,s3y);
			#endif
			ScanTwoEdges(p0x,p0y,p1x,p1y,p1x,p1y,p3x,p3y,edgeBuffer+1);
			}
		}

#undef INTERPOLATE_EDGE_XMAJOR
#undef INTERPOLATE_EDGE_YMAJOR
#undef EGDE_INTERPOLATION
#undef ScanEdge
#undef ScanTwoEdges
#undef SETUP_SECOND_UV
#undef SET_UV
#undef SET_UV_TWO
#undef INT_SETUP_UV
#undef INT_UPDATE_UV
#undef INT_ASSIGN_UV
