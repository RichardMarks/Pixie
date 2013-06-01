#ifndef __PodCheck_H__
#define __PodCheck_H__

enum POD_ENUM { IS_POD, NOT_POD };
template< typename T, POD_ENUM P> struct MUST_BE_POD { MUST_BE_POD() { } };
template< typename T > struct MUST_BE_POD< T, IS_POD > { MUST_BE_POD() { union T_must_be_POD { T a; T b; }; } };
#define COMPILE_CHECK_IS_POD( TYPE, POD_ENUM ) do { MUST_BE_POD< TYPE, POD_ENUM > POD_TEST; POD_TEST; } while( 0 )


#endif /* __PodCheck_H__ */