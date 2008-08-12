#ifndef _ABSTRACT_IMAGE_FILTER_WHOLEATONCE_H
#error File AbstractImageFilterWholeAtOnce.tcc cannot be included directly!
#else

namespace M4D
{
namespace Imaging
{

template< typename InputImageType, typename OutputImageType >
bool
AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType >
::ExecutionThreadMethod( AbstractPipeFilter::UPDATE_TYPE utype )
{
	if ( !( _readerBBox->WaitWhileDirty() == MS_MODIFIED ) ) {
		_writerBBox->SetState( MS_CANCELED );
		return false;
	}

	if( ProcessImage( *(this->in), *(this->out) ) ) {
		_writerBBox->SetModified();
		return true;
	} else {
		_writerBBox->SetState( MS_CANCELED );
		return false;
	}
}

template< typename InputImageType, typename OutputImageType >
void
AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType >
::BeforeComputation( AbstractPipeFilter::UPDATE_TYPE &utype )
{
	PredecessorType::BeforeComputation( utype );
	
	//This kind of filter computes always on whole dataset
	utype = AbstractPipeFilter::RECALCULATION;

	_readerBBox = ApplyReaderBBox( *(this->in) );
	_writerBBox = ApplyWriterBBox( *(this->out) );
	
}

template< typename InputImageType, typename OutputImageType >
void
AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType >
::AfterComputation( bool successful )
{
	_readerBBox = ReaderBBoxInterface::Ptr();
	_writerBBox = NULL;

	PredecessorType::AfterComputation( successful );
}
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
template< typename ElementType, unsigned dim >
ReaderBBoxInterface::Ptr
ApplyReaderBBox(  const Image< ElementType, dim > &in );

template< typename ElementType, unsigned dim >
ReaderBBoxInterface::Ptr
ApplyReaderBBoxFunc(  const Image< ElementType, 2 > &in )
{
	return in.GetDirtyBBox( 
				in.GetDimensionExtents( 0 ).minimum,
				in.GetDimensionExtents( 1 ).minimum,
				in.GetDimensionExtents( 0 ).maximum,
				in.GetDimensionExtents( 1 ).maximum
			);
}

template< typename ElementType, unsigned dim >
ReaderBBoxInterface::Ptr
ApplyReaderBBoxFunc(  const Image< ElementType, 3 > &in )
{
	return in.GetDirtyBBox( 
				in.GetDimensionExtents( 0 ).minimum,
				in.GetDimensionExtents( 1 ).minimum,
				in.GetDimensionExtents( 2 ).minimum,
				in.GetDimensionExtents( 0 ).maximum,
				in.GetDimensionExtents( 1 ).maximum,
				in.GetDimensionExtents( 2 ).maximum
			);
}

template< typename ElementType, unsigned dim >
ReaderBBoxInterface::Ptr
ApplyReaderBBoxFunc(  const Image< ElementType, 4 > &in )
{
	return in.GetDirtyBBox( 
				in.GetDimensionExtents( 0 ).minimum,
				in.GetDimensionExtents( 1 ).minimum,
				in.GetDimensionExtents( 2 ).minimum,
				in.GetDimensionExtents( 3 ).minimum,
				in.GetDimensionExtents( 0 ).maximum,
				in.GetDimensionExtents( 1 ).maximum,
				in.GetDimensionExtents( 2 ).maximum,
				in.GetDimensionExtents( 3 ).maximum
			);
}

template< typename ElementType, unsigned dim >
WriterBBoxInterface *
ApplyWriterBBoxFunc(  const Image< ElementType, dim > &out );

template< typename ElementType, unsigned dim >
WriterBBoxInterface *
ApplyWriterBBoxFunc(  const Image< ElementType, 2 > &out )
{
	return out.SetDirtyBBox( 
				out.GetDimensionExtents( 0 ).minimum,
				out.GetDimensionExtents( 1 ).minimum,
				out.GetDimensionExtents( 0 ).maximum,
				out.GetDimensionExtents( 1 ).maximum
			);
}

template< typename ElementType, unsigned dim >
WriterBBoxInterface *
ApplyWriterBBoxFunc(  const Image< ElementType, 3 > &out )
{
	return out.SetDirtyBBox( 
				out.GetDimensionExtents( 0 ).minimum,
				out.GetDimensionExtents( 1 ).minimum,
				out.GetDimensionExtents( 2 ).minimum,
				out.GetDimensionExtents( 0 ).maximum,
				out.GetDimensionExtents( 1 ).maximum,
				out.GetDimensionExtents( 2 ).maximum
			);
}

template< typename ElementType, unsigned dim >
WriterBBoxInterface *
ApplyWriterBBoxFunc(  const Image< ElementType, 4 > &out )
{
	return out.SetDirtyBBox( 
				out.GetDimensionExtents( 0 ).minimum,
				out.GetDimensionExtents( 1 ).minimum,
				out.GetDimensionExtents( 2 ).minimum,
				out.GetDimensionExtents( 3 ).minimum,
				out.GetDimensionExtents( 0 ).maximum,
				out.GetDimensionExtents( 1 ).maximum,
				out.GetDimensionExtents( 2 ).maximum,
				out.GetDimensionExtents( 3 ).maximum
			);
}
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
template< typename InputImageType, typename OutputImageType >
ReaderBBoxInterface::Ptr
AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType >
::ApplyReaderBBox( const InputImageType &in )
{
	return ApplyReaderBBoxFunc< InputImageType::Element, InputImageType::Dimension >( in );
}

template< typename InputImageType, typename OutputImageType >
WriterBBoxInterface *
AbstractImageFilterWholeAtOnce< InputImageType, OutputImageType >
::ApplyWriterBBox( OutputImageType &out )
{
	return ApplyWriterBBoxFunc< OutputImageType::Element, OutputImageType::Dimension >( out );
}


} /*namespace Imaging*/
} /*namespace M4D*/

#endif /*_ABSTRACT_IMAGE_FILTER_WHOLEATONCE_H*/
