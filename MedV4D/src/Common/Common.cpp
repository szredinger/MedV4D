/**
 *  @ingroup common
 *  @file Common.cpp
 *  @author Jan Kolomaznik
 */
#include "MedV4D/Common/Common.h"

namespace M4D
{

const float32 Epsilon = 1.0E-10f;
const float32 PI = 3.141592f;
const float32 PIx2 = PI*2;
const float32 PIx3 = PI*3;
const float32 PIx4 = PI*4;
const float32 PId2 = PI/2;
const float32 PId3 = PI/3;
const float32 PId4 = PI/4;

}//namespace M4D

#include "MedV4D/Common/Direction.h"

namespace M4D
{

Vector<int32,2>	directionOffset[] = {
	Vector<int32,2>( 1, 0 ),
	Vector<int32,2>( 1, -1 ),
	Vector<int32,2>( 0, -1 ),
	Vector<int32,2>( -1, -1 ),
	Vector<int32,2>( -1, 0 ),
	Vector<int32,2>( -1, 1 ),
	Vector<int32,2>( 0, 1 ),
	Vector<int32,2>( 1, 1 )
	};

}//namespace M4D

#include "MedV4D/Common/Types.h"

float32 TypeTraits< float32 >::Max = MAX_FLOAT32;
float32 TypeTraits< float32 >::Min = -MAX_FLOAT32;
float32 TypeTraits< float32 >::Zero = 0.0f;
float32 TypeTraits< float32 >::One = 1.0f;
float32 TypeTraits< float32 >::CentralValue = 0.0f;

float64 TypeTraits< float64 >::Max = MAX_FLOAT64;
float64 TypeTraits< float64 >::Min = -MAX_FLOAT64;
float64 TypeTraits< float64 >::Zero = 0.0;
float64 TypeTraits< float64 >::One = 1.0;
float64 TypeTraits< float64 >::CentralValue = 0.0;

#define INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( TTYPE, DIM ) \
		template<> Vector< TTYPE, DIM > TypeTraits< Vector< TTYPE, DIM > >::Max = Vector< TTYPE, DIM >( TypeTraits< TTYPE >::Max ); \
		template<> Vector< TTYPE, DIM > TypeTraits< Vector< TTYPE, DIM > >::Min = Vector< TTYPE, DIM >( TypeTraits< TTYPE >::Min ); \
		template<> Vector< TTYPE, DIM > TypeTraits< Vector< TTYPE, DIM > >::Zero = Vector< TTYPE, DIM >( TypeTraits< TTYPE >::Zero ); \
		template<> Vector< TTYPE, DIM > TypeTraits< Vector< TTYPE, DIM > >::CentralValue = Vector< TTYPE, DIM >( TypeTraits< TTYPE >::CentralValue ); \
		template<> TTYPE TypeTraits< Vector< TTYPE, DIM > >::One = TypeTraits< TTYPE >::One;


INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int8, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint8, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int16, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint16, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int32, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint32, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int64, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint64, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float32, 2 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float64, 2 )

INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int8, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint8, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int16, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint16, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int32, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint32, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int64, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint64, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float32, 3 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float64, 3 )

INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int8, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint8, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int16, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint16, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int32, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint32, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( int64, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( uint64, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float32, 4 )
INITIALIZE_SIMPLE_VECTOR_CONSTANTS_MACRO( float64, 4 )

//TODO - platform independend.
int16
GetNTIDFromSizeAndSign( uint16 size, bool sign )
{
	switch ( size ) {
	case 1:
		return ( sign ? NTID_INT_8 : NTID_UINT_8 );
	case 2:
		return ( sign ? NTID_INT_16 : NTID_UINT_16 );
	case 4:
		return ( sign ? NTID_INT_32 : NTID_UINT_32 );
	case 8:
		return ( sign ? NTID_INT_64 : NTID_UINT_64 );
	default:
		return (int16)NTID_UNKNOWN;
	}
}

uint32
GetByteCountFromNTID( int16 ntid )
{
	TYPE_TEMPLATE_SWITCH_MACRO( ntid, return sizeof( TTYPE ); );
	return 0;
}

#ifdef USE_DEVIL
#include <IL/il.h>
#include <IL/ilu.h>
#endif /*USE_DEVIL*/	

void
Medv4DInit()
{
#ifdef USE_DEVIL
	ilInit();
	iluInit();
#endif /*USE_DEVIL*/	

#ifdef USE_TBB

#endif /*USE_TBB*/

}



