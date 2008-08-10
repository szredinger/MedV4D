#ifndef _THRESHOLDING_FILTER_H
#error File ConvolutionFilter.tcc cannot be included directly!
#else

namespace M4D
{

namespace Imaging
{

template< typename InputImageType, typename MatrixElement >
ConvolutionFilter2D< InputImageType, MatrixElement >::Properties
::Properties() : PredecessorType::Properties( 0, 10 ), width( 1 ), height( 1 )
{
	matrix = MatrixPtr( new MatrixElement[1] );

	matrix[0] = 1;
}

template< typename InputElementType, typename MatrixElement >
ConvolutionFilter2D< Image< InputElementType, 3 >, MatrixElement >
::ConvolutionFilter2D() : PredecessorType( new Properties() )
{

}

template< typename InputElementType, typename MatrixElement >
ConvolutionFilter2D< Image< InputElementType, 3 >, MatrixElement >
::ConvolutionFilter2D( ConvolutionFilter2D< Image< InputElementType, 3 >, MatrixElement >::Properties *prop ) 
: PredecessorType( prop ), 
{

}

template< typename InputElementType, typename MatrixElement >
bool
ConvolutionFilter2D< Image< InputElementType, 3 >, MatrixElement >
::ProcessSlice(	
			const Image< InputElementType, 3 > 	&in,
			Image< InputElementType, 3 >		&out,
			size_t			x1,	
			size_t			y1,	
			size_t			x2,	
			size_t			y2,	
			size_t			slice
		    )
{
	int hwidth = GetProperties().width / 2;
	int hheight = GetProperties().height / 2;

	//TODO check
	for( size_t i = x1 + hwidth; i < ( x2 - GetProperties().width + hwidth ); ++i ) {
		for( size_t j = y1 + hheight; j < ( y2 - GetProperties().height + hheight ); ++j ) {
			MatrixElement tmp = 0.0;	
			for( size_t ii = 0; ii < GetProperties().width; ++ii ) {
				for( size_t jj = 0; jj < GetProperties().height; ++jj ) {
					tmp += GetProperties().matrix[ GetProperties().width * jj + ii ] 
						* in.GetElement( i + ii - hwidth, j + jj - hheight, slice );
				}
			}
			out.GetElement( i, j, value ) = static_cast<InputElementType>( tmp );
		}
	}
}

//******************************************************************************
//******************************************************************************

template< typename InputImageType, typename MatrixElement >
ConvolutionFilter3D< InputImageType, MatrixElement >::Properties
::Properties() : PredecessorType::Properties( 0, 10 ), width( 1 ), height( 1 ), depth( 1 )
{
	matrix = MatrixPtr( new MatrixElement[1] );

	matrix[0] = 1;
}

template< typename InputElementType >
ConvolutionFilter3D< Image< InputElementType, 3 > >
::ConvolutionFilter3D() : public PredecessorType( 0, 15 )
{

}

template< typename InputElementType >
bool
ConvolutionFilter3D< Image< InputElementType, 3 > >
::ProcessSlice(	
			const Image< InputElementType, 3 > 	&in,
			Image< InputElementType, 3 >		&out,
			size_t			x1,	
			size_t			y1,	
			size_t			x2,	
			size_t			y2,	
			size_t			slice
		    )
{
	for( size_t i = x1; i < x2; ++i ) {
		for( size_t j = y1; j < y2; ++j ) {
			InputElementType value = in.GetElement( i, j, slice );
			if( GetProperties().bottom <= value && GetProperties().top >= value ) {
				//unchanged
			} else {
				out.GetElement( i, j, value ) = GetProperties().outValue;
			}
		}
	}
}

} /*namespace Imaging*/
} /*namespace M4D*/

#endif /*_THRESHOLDING_FILTER_H*/
