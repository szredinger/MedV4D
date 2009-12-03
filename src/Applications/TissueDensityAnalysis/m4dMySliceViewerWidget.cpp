/**
 *  @ingroup gui
 *  @file m4dGUISliceViewerWidget2.cpp
 *  @brief some brief
 */

#include "m4dMySliceViewerWidget.h"
#include "GUI/widgets/components/RGBSliceViewerTexturePreparer.h"


namespace M4D
{
namespace Viewer
{


template< typename ElementType >
bool
MySimpleSliceViewerTexturePreparer< ElementType >
::prepare( const Imaging::InputPortList& inputPorts,
      uint32& width,
      uint32& height,
      GLint brightnessRate,
      GLint contrastRate,
      SliceOrientation so,
      uint32 slice,
      unsigned& dimension )
    {

	// get the input datasets
	ElementType** pixel = getDatasetArrays( inputPorts, 1, width, height, so, slice, dimension );

	if ( ! *pixel )
	{
	    delete[] pixel;
	    return false;
	}

	// equalize the first input array
	adjustArrayContrastBrightness( *pixel, width, height, brightnessRate, contrastRate );

	// prepare texture
        glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
                      GL_LUMINANCE, this->oglType(), *pixel );

	// free temporary allocated space
	delete[] *pixel;

	delete[] pixel;

        return true;
    }


template< typename ElementType >
ElementType**
MySimpleSliceViewerTexturePreparer< ElementType >
::getDatasetArrays( const Imaging::InputPortList& inputPorts,
      uint32 numberOfDatasets,
      uint32& width,
      uint32& height,
      SliceOrientation so,
      uint32 slice,
      unsigned& dimension )
    {
	uint32 i, tmpwidth, tmpheight;
	Imaging::InputPortTyped<Imaging::AImage>* inPort;
	ElementType** result = new ElementType*[ numberOfDatasets ];

	width = height = 0;

	// loop through the input ports
	for ( i = 0; i < numberOfDatasets; i++ )
	{
	    if ( inputPorts.Size() <= i ) result[i] = NULL;
	    else
	    {
			tmpwidth = tmpheight = 0;

			// get the port and drag the data out of the port
			inPort = inputPorts.GetPortTypedSafe< Imaging::InputPortTyped<Imaging::AImage> >( i );
			result[i] = this->prepareSingle( inPort, tmpwidth, tmpheight, so, slice, dimension );
			if ( result[i] && ( ( tmpwidth < width && tmpwidth > 0 ) || width == 0 ) ) width = tmpwidth;
			if ( result[i] && ( ( tmpheight < height && tmpheight > 0 ) || height == 0 ) ) height = tmpheight;	
	    }
	}

	return result;

    }

void 
SliceViewerSpecialStateOperator::SelectMethodLeft( double x, double y, int sliceNum, double zoomRate )
{
	//m4dMySliceViewerWidget::sphereCenter(x,y,sliceNum);
	//int a = 5
	//emit m4dMySliceViewerWidget::signalSphereCenter(x,  y,  sliceNum);
}


m4dMySliceViewerWidget::m4dMySliceViewerWidget( 
  Imaging::ConnectionInterface* conn, 
  Imaging::ConnectionInterface* connMask, unsigned index, QWidget *parent)
    : m4dGUISliceViewerWidget(parent, index)
{
    //TODO: smazat port list
    _index = index;
    _inPort = new Imaging::InputPortTyped<Imaging::AImage>();
    _inMaskPort = new Imaging::InputPortTyped<Imaging::AImage>();
    resetParameters();
    _inputPorts.AppendPort( _inPort );
    _inputPorts.AppendPort( _inMaskPort );

    conn->ConnectConsumer( *_inPort );
    connMask->ConnectConsumer( *_inMaskPort );
}

void 
m4dMySliceViewerWidget::specialStateButtonMethodLeft( int amountA, int amountB )
{
	if( _specialState ) {
		//_specialState->ButtonMethodLeft( amountA, amountB, _zoomRate );
//		emit signalSphereRadius( amountA, amountB, _zoomRate);
	}
}
/*
void 
m4dMySliceViewerWidget::specialStateButtonMethodRight( int amountA, int amountB )
{
	if( _specialState ) {
		_specialState->ButtonMethodRight( amountA, amountB, _zoomRate );
	}
}
*/
void 
m4dMySliceViewerWidget::specialStateSelectMethodLeft( double x, double y, double z )
{
	if( _specialState ) {
		resolveFlips( x, y );
		int sliceNum = z/_extents[2];
		emit signalSphereCenter(x,  y,  (double)sliceNum);
		//_specialState->SelectMethodLeft( x + _extents[0]*_minimum[0], y + _extents[1]*_minimum[1], sliceNum, _zoomRate );
	}
}
/*
void 
m4dMySliceViewerWidget::specialStateSelectMethodRight( double x, double y, double z )
{
	if( _specialState ) {
		resolveFlips( x, y );
		int sliceNum = z/_extents[2];
		_specialState->SelectMethodRight( x + _extents[0]*_minimum[0], y + _extents[1]*_minimum[1], sliceNum, _zoomRate );
	}
}

void 
m4dMySliceViewerWidget::drawSliceAdditionals( int sliceNum, double zoomRate )
{
	if( _specialState ) {
		glPushMatrix();

		glTranslatef( -_extents[0]*_minimum[0], -_extents[1]*_minimum[1], 0.0f );

		_specialState->Draw( *this, sliceNum, zoomRate );
		
		glPopMatrix();
	}
	PredecessorType::drawSliceAdditionals( sliceNum, zoomRate );
}
*/
void 
m4dMySliceViewerWidget::drawHUD( int sliceNum, double zoomRate, QPoint offset )
{
	PredecessorType::drawHUD( sliceNum, zoomRate, offset );
}
/*
void 
m4dMySliceViewerWidget::makeConnections( InputPort& inputPort)
{
	_inputPorts.AppendPort(inputPort);
}
*/
void 
m4dMySliceViewerWidget::setButtonHandler( ButtonHandler hnd, MouseButton btn )
{
	if( hnd != specialState ) {
		PredecessorType::setButtonHandler( hnd, btn );
		return;
	}

	_selectMethods[ left ] = (SelectMethods)&M4D::Viewer::m4dMySliceViewerWidget::specialStateSelectMethodLeft;
	_selectMode[ left ] = true;
	/*_selectMethods[ right ] = (SelectMethods)&M4D::Viewer::m4dMySliceViewerWidget::specialStateSelectMethodRight;
	_selectMode[ right ] = true;*/

	_buttonMethods[ left ] = (ButtonMethods)&M4D::Viewer::m4dMySliceViewerWidget::specialStateButtonMethodLeft;
	_buttonMode[ left ] = true;/*
	_buttonMethods[ right ] = (ButtonMethods)&M4D::Viewer::m4dMySliceViewerWidget::specialStateButtonMethodRight;
	_buttonMode[ right ] = true;
*/
	if ( _ready ) updateGL();
	emit signalSetButtonHandler( _index, hnd, btn );
}
/*
void 
m4dMySliceViewerWidget::mousePressEvent(QMouseEvent *event) {
  MessageBox(NULL, L"AHOJ", L"AHOJ",MB_OK);
}*/

void
m4dMySliceViewerWidget::sphereCenter( double x, double y, double z )
{
    double w, h;
  /*  calculateWidthHeight( w, h );
    if ( !_inPort->IsPlugged() ) return;
    double coords[3];
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    if ( checkOutOfBounds( coords[ _sliceOrientation ], coords[ ( _sliceOrientation + 1 ) % 3 ] ) ) return;

    Selection::m4dShape<double> s( 3, _sliceOrientation );
    _shapes.push_back( s );
    newPoint( x, y, z );
    resolveFlips( coords[ _sliceOrientation ], coords[ ( _sliceOrientation + 1 ) % 3 ] );*/
    //emit signalSphereCenter( _index, coords[0], coords[1], coords[2] );
}

void m4dMySliceViewerWidget::slotSetSpecialStateSelectMethodLeft()
{
	setButtonHandler( specialState, left );
	SliceViewerSpecialStateOperator *sState = new SliceViewerSpecialStateOperator();
	_specialState = M4D::Viewer::SliceViewerSpecialStateOperatorPtr( sState );
}



void
m4dMySliceViewerWidget::drawSlice( int sliceNum, double zoomRate, QPoint offset )
{
    if ( !_ready ) return;
    if ( !_inPort->IsPlugged() ) return;
    double w, h;
    if ( sliceNum < (int)_minimum[ ( _sliceOrientation + 2 ) % 3 ] ||
         sliceNum >= (int)_maximum[ ( _sliceOrientation + 2 ) % 3 ] )
    {
        return;
    }
    else
    {
	calculateWidthHeight( w, h );
    }
    glLoadIdentity();
    
    // manage flips
    if ( _flipH < 0 ) offset.setX( offset.x() + (int)( zoomRate * w ) );
    if ( _flipV < 0 ) offset.setY( offset.y() + (int)( zoomRate * h ) );
    uint32 height, width;
    GLuint texName;

    // opengl texture setup functions
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures( 1, &texName );

    glBindTexture ( GL_TEXTURE_2D, texName );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glTranslatef( offset.x(), offset.y(), 0 );
    glScalef( _flipH * zoomRate, _flipV * zoomRate, 0. );
    
    // prepare texture
    if ( texturePreparerType == custom || texturePreparerType == rgb )
	for ( uint32 i = 1; i < SLICEVIEWER_INPUT_NUMBER; i++ )
	{
	    if ( this->InputPort()[i].IsPlugged() ) break;
	    if ( i == SLICEVIEWER_INPUT_NUMBER - 1 ) texturePreparerType = simple;
	}

    switch ( texturePreparerType )
    {

	case rgb:
        NUMERIC_TYPE_TEMPLATE_SWITCH_MACRO(
    	    _imageID, 
			{ 
				RGBSliceViewerTexturePreparer<TTYPE> texturePreparer; 
				_ready = texturePreparer.prepare( 
						this->InputPort(), 
						width, 
						height, 
						_brightnessRate, 
						_contrastRate, 
						_sliceOrientation, 
						sliceNum - _minimum[ ( _sliceOrientation + 2 ) % 3 ], 
						_dimension 
						); 
			}
		);
	break;

	case custom:
	_ready = customTexturePreparer->prepare( this->InputPort(), width, height, _brightnessRate, _contrastRate, _sliceOrientation, sliceNum - _minimum[ ( _sliceOrientation + 2 ) % 3 ], _dimension );
	break;

	default:
		NUMERIC_TYPE_TEMPLATE_SWITCH_MACRO(
				_imageID, 
				{
					MySimpleSliceViewerTexturePreparer<TTYPE> texturePreparer;
					_ready = texturePreparer.prepare( 
						this->InputPort(), 
						width, 
						height, 
						_brightnessRate, 
						_contrastRate, 
						_sliceOrientation, 
						sliceNum - _minimum[ ( _sliceOrientation + 2 ) % 3 ], 
						_dimension ); 
				} 
		);
        break;

    }

    if ( !_ready ) return;
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texName );

    // draw surface and map texture on it
    glBegin( GL_QUADS );
        glTexCoord2d(0.0,0.0); glVertex2d(  0.0,   0.0);
        glTexCoord2d(1.0,0.0); glVertex2d(width * _extents[ _sliceOrientation ],   0.0);
        glTexCoord2d(1.0,1.0); glVertex2d(width * _extents[ _sliceOrientation ], height * _extents[ ( _sliceOrientation + 1 ) % 3 ] );
        glTexCoord2d(0.0,1.0); glVertex2d(  0.0,height * _extents[ ( _sliceOrientation + 1 ) % 3 ] );
        glEnd();
    glDeleteTextures( 1, &texName );
    
	drawSliceAdditionals( sliceNum, zoomRate );

	if ( _flipH < 0 ) offset.setX( offset.x() - (int)( zoomRate * w ) );
	if ( _flipV < 0 ) offset.setY( offset.y() - (int)( zoomRate * h ) );

	drawHUD( sliceNum, zoomRate, offset );
/*
    // if there are selected shapes, draw them
    if ( !_shapes.empty() )
    {
        for ( std::list< Selection::m4dShape<double> >::iterator it = _shapes.begin(); it != --(_shapes.end()); ++it )
            drawShape( *it, false, sliceNum, zoomRate );
        drawShape( *(--(_shapes.end())), true, sliceNum, zoomRate );
    }
    if ( _flipH < 0 ) offset.setX( offset.x() - (int)( zoomRate * w ) );
    if ( _flipV < 0 ) offset.setY( offset.y() - (int)( zoomRate * h ) );
    
    // print text data if requested
    if ( _printData ) drawData( zoomRate, offset, sliceNum );

    // print color value of the picked voxel
    if ( _colorPicker && sliceNum == _slicePicked ) drawPicked();
*/

    glFlush();
}

} /*namespace Viewer*/
} /*namespace M4D*/
