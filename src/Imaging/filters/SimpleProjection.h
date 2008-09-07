#ifndef _SIMPLE_PROJECTION_H
#define _SIMPLE_PROJECTION_H

#include "Common.h"
#include "Imaging/AbstractImageFilterWholeAtOnce.h"

namespace M4D
{

/**
 * @ingroup imaging 
 * @author Jan Kolomaznik 
 * @file SimpleProjection.h 
 * @{ 
 **/

namespace Imaging
{

enum CartesianPlanes{
	XY_PLANE,
	XZ_PLANE,
	YZ_PLANE
};	

enum ProjectionType{
	PT_MAX,
	PT_SUM,
	PT_AVERAGE
};

template< typename ImageType >
class SimpleProjection;

template< typename ElementType >
class SimpleProjection< Image< ElementType, 3 > >
	: public AbstractImageFilterWholeAtOnce< Image< ElementType, 3 >, Image< ElementType, 2 > >
{
public:	
	typedef Image< ElementType, 3 >		InputImageType;
	typedef Image< ElementType, 2 >		OutputImageType;
	typedef AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType > PredecessorType;

	struct Properties : public PredecessorType::Properties
	{
		Properties(): plane( XY_PLANE ), projectionType( PT_MAX ) {}

		CartesianPlanes	plane;
		ProjectionType	projectionType;
	};

	SimpleProjection( Properties * prop );
	SimpleProjection();

	GET_SET_PROPERTY_METHOD_MACRO( CartesianPlanes, Plane, plane );
	GET_SET_PROPERTY_METHOD_MACRO( ProjectionType, ProjectionType, projectionType );
protected:

	void
	BeforeComputation( AbstractPipeFilter::UPDATE_TYPE &utype );

	bool
	ProcessImage(
			const InputImageType 	&in,
			OutputImageType		&out
		    );

	template< typename OperatorType >
	bool
	ProcessImageHelper(
			const InputImageType 	&in,
			OutputImageType		&out
		    );

	void
	PrepareOutputDatasets();

	template< typename OperatorType >
	bool
	DoProjection(
			ElementType	*inPointer,
			ElementType	*outPointer,
			int32		ixStride,
			int32		iyStride,
			int32		izStride,
			int32		oxStride,
			int32		oyStride,
			uint32		width,
			uint32		height,
			uint32		depth
		    );


private:
	GET_PROPERTIES_DEFINITION_MACRO;

};

template< typename ElementType >
struct MaxOperator
{
	void
	Init( ElementType &initValue )
		{ max = initValue; }

	void
	AddElement( ElementType &value )
		{ if( value > max ) { max = value; } }

	ElementType
	Result()
		{ return max; }

	ElementType max;
};

template< typename ElementType >
struct SumOperator
{
	void
	Init( ElementType &initValue )
		{ sum = initValue; }

	void
	AddElement( ElementType &value )
		{ sum += value; }

	ElementType
	Result()
		{ return (sum > TypeTraits< ElementType >::Max) ? TypeTraits< ElementType >::Max : sum; }

	int64 sum;
};

template< typename ElementType >
struct AverageOperator
{
	void
	Init( ElementType &initValue )
		{ sum = initValue; count = 1; }

	void
	AddElement( ElementType &value )
		{ sum += value; ++count; }

	ElementType
	Result()
		{ return sum / count; }

	int64		sum;
	uint32		count;
};

//******************************************************************************
//******************************************************************************


} /*namespace Imaging*/
/** @} */

} /*namespace M4D*/


//include implementation
#include "Imaging/filters/SimpleProjection.tcc"

#endif /*_SIMPLE_PROJECTION_H*/

