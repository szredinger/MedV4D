/**
 * @ingroup cellbe 
 * @author Vaclav Klecanda 
 * @file SimpleMaxIntensityProjSerializer.h 
 * @{ 
 **/

#ifndef MAXINTENSITY_SERIALIZER_H
#define MAXINTENSITY_SERIALIZER_H

#include "cellBE/netStream.h"
#include "cellBE/AbstractFilterSerializer.h"
#include "Imaging/filters/SimpleMaxIntensityProjection.h"

namespace M4D
{
namespace CellBE
{

// supportig function
template< typename ElementType >
void
CreateMIFilter( 
     M4D::Imaging::AbstractPipeFilter **resultingFilter
   , AbstractFilterSerializer **serializer
   , const uint16 id
   , M4D::CellBE::NetStream &s )
{
	typedef M4D::Imaging::Image< ElementType, 3 > ImageType;
  typedef M4D::Imaging::SimpleMaxIntensityProjection< ImageType > Filter;
  	typedef FilterSerializer< Filter > FilterSerializer;

	typename Filter::Properties *prop = new typename Filter::Properties();

	*resultingFilter = new Filter( prop );
  *serializer = new FilterSerializer( prop, id);
}

/**
 *  MaxIntensitySerializer.
 */
template< typename InputImageType >
class FilterSerializer< M4D::Imaging::SimpleMaxIntensityProjection< InputImageType > > 
	: public AbstractFilterSerializer
{
public:
	typedef typename M4D::Imaging::SimpleMaxIntensityProjection< InputImageType >
    ::Properties Properties;
	
	FilterSerializer( Properties * props, uint16 id) 
		: AbstractFilterSerializer( FID_Median, id )
		, _properties( props ) 
  {}

  void SerializeClassInfo( M4D::CellBE::NetStream &s)
  {
		s << (uint8) GetNumericTypeID< typename M4D::Imaging::ImageTraits< InputImageType >::ElementType >();
  }

  void
  DeSerializeClassInfo( 
      M4D::Imaging::AbstractPipeFilter **resultingFilter
    , AbstractFilterSerializer **serializer
    , const uint16 id
    , M4D::CellBE::NetStream &s
    )
  {
		uint8 typeID;
		s >> typeID;
		
		NUMERIC_TYPE_TEMPLATE_SWITCH_MACRO( typeID, 
			CreateMIFilter<TTYPE>( resultingFilter, serializer, id, s )
		);
  }

	void 
	SerializeProperties( M4D::CellBE::NetStream &s)
	{		
		s << (uint8)_properties->plane;
	}

	void
	DeSerializeProperties( M4D::CellBE::NetStream &s )
	{
		s >> (uint8&)_properties->plane;
	}	
	
protected:
	Properties	*_properties;
};

}
}

#endif


/** @} */

