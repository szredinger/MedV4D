#ifdef USE_CG

#ifndef BASIC_SLICE_VIEWER_H
#define BASIC_SLICE_VIEWER_H


#include "GUI/utils/CgShaderTools.h"
#include "GUI/utils/GLTextureImage.h"
#include "GUI/utils/FrameBufferObject.h"
#include "GUI/widgets/AGUIViewer.h"
#include "GUI/widgets/ViewerConstructionKit.h"
#include <QtGui>
#include <QtOpenGL>
#include <boost/shared_ptr.hpp>
#include "Imaging/Imaging.h"
#include "GUI/utils/ViewConfiguration.h"
//#include "GUI/utils/ImageDataRenderer.h"
#include "GUI/utils/TransferFunctionBuffer.h"
#include "GUI/utils/IUserEvents.h"
#include <map>

#include "GUI/renderers/SliceRenderer.h"
#include "GUI/renderers/VolumeRenderer.h"


namespace M4D
{
namespace GUI
{
namespace Viewer
{

enum RendererType
{
	rt2DAlignedSlices,
	rt3DGeneralSlices,
	rt3D
};

class RenderingThread: public QThread
{
public:
	RenderingThread(): mSize( 100, 100 )
	{

	}
	void
	SetContext( QGLContext &aContext )
	{
		mContext = &aContext;
	}

	void
	Initialize()
	{
		mContext->makeCurrent();
		GL_CHECKED_CALL( glGenFramebuffersEXT( 1, &mFrameBufferObject ) );
		GL_CHECKED_CALL( glGenRenderbuffersEXT( 1, &mDepthBuffer ) );

		GL_CHECKED_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFrameBufferObject ) );
		GL_CHECKED_CALL( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mDepthBuffer ) );

		GL_CHECKED_CALL( glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mSize.width(), mSize.height() ) );

		GL_CHECKED_CALL( glFramebufferRenderbufferEXT( 
					GL_FRAMEBUFFER_EXT,
					GL_DEPTH_ATTACHMENT_EXT,
					GL_RENDERBUFFER_EXT,
					mDepthBuffer
					) );
	}

	void
	Finalize()
	{
		mContext->makeCurrent();
		GL_CHECKED_CALL( glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 ) );
		GL_CHECKED_CALL( glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 ) );

		GL_CHECKED_CALL( glDeleteRenderbuffersEXT( 1, &mDepthBuffer ) );
		GL_CHECKED_CALL( glDeleteFramebuffersEXT( 1, &mFrameBufferObject ) );
	}
protected:
	virtual void
	run()
	{

		

		

		/*GL_CHECKED_CALL( glFramebufferTexture2DEXT( 
					GL_FRAMEBUFFER_EXT,
					GL_COLOR_ATTACHMENT0_EXT,
					GL_TEXTURE_2D,
					mColorTexture,
					0 
					) );*/
		while ( true ) {
			mContext->makeCurrent();



		}
		

	}
	GLuint	mFrameBufferObject, 
		mDepthBuffer, 
		mColorTexture;
	QGLContext	*mContext;
	QSize	mSize;
};


class BasicSliceViewer : 
	public ViewerConstructionKit<   QGLWidget, 
					PortInterfaceHelper< boost::mpl::vector< M4D::Imaging::AImage > >
					>
{
	Q_OBJECT;
public:
	typedef ViewerConstructionKit<  QGLWidget, 
					PortInterfaceHelper< boost::mpl::vector< M4D::Imaging::AImage > >
					>	PredecessorType;
	

	BasicSliceViewer( QWidget *parent = NULL );

	~BasicSliceViewer();

	void
	SetLUTWindow( float32 center, float32 width );

	void
	SetLUTWindow( Vector2f window );

	void
	SetTransferFunctionBuffer( TransferFunctionBuffer1D::Ptr aTFunctionBuffer );

	/*const Vector2f &
	GetLUTWindow()
		{ return _lutWindow; }*/

	void
	SetCurrentSlice( int32 slice );

	void
	ZoomFit( ZoomType zoomType = ztFIT );


	bool
	IsColorTransformAvailable( unsigned aTransformType );

	int
	GetRendererType()
	{
		return mRendererType;//_renderer.GetRendererType();
	}

	int
	GetColorTransformType()
	{
		return mSliceRenderConfig.colorTransform;//_renderer.GetColorTransformType();
	}


	void
	ReceiveMessage( 
		M4D::Imaging::PipelineMessage::Ptr 			msg, 
		M4D::Imaging::PipelineMessage::MessageSendStyle 	sendStyle, 
		M4D::Imaging::FlowDirection				direction
	)
	{
		PrepareData();
	}

	bool
	IsShadingEnabled() const
	{
		return mVolumeRenderConfig.shadingEnabled;
	}

	bool
	IsJitteringEnabled() const
	{
		return mVolumeRenderConfig.jitterEnabled;
	}

public slots:
	void
	SetRendererType( int aRendererType )
	{
		mRendererType = aRendererType;
		//TODO 

		mCurrentEventHandler = mUserEventHandlers[ aRendererType ].get();
		update();

		emit RendererTypeChanged( aRendererType );
	}

	void
	SetColorTransformType( int aColorTransform )
	{
		//TODO 
		mSliceRenderConfig.colorTransform = aColorTransform;
		mVolumeRenderConfig.colorTransform = aColorTransform;

		update();

		emit ColorTransformTypeChanged( aColorTransform );
	}

	void
	FineRender()
	{
		//_renderer.FineRender();
		update();
	}

	void
	EnableShading( bool aEnable )
	{
		mVolumeRenderConfig.shadingEnabled = aEnable;
		update();
	}

	void
	EnableJittering( bool aEnable )
	{
		mVolumeRenderConfig.jitterEnabled = aEnable;
		update();
	}

	void
	SaveCurrentView()
	{
		mSaveFile = true;
		FineRender();
	}

	void
	SaveCycle()
	{
		mSaveCycle = true;
		update();
	}
	
	void
	ResetView()
	{
		Vector3f pos = mVolumeRenderConfig.camera.GetTargetPosition();
		pos[1] += -550;
		mVolumeRenderConfig.camera.SetEyePosition( pos, Vector3f( 0.0f, 0.0f, 1.0f ) );
		
		update();
	}
signals:
	void
	SettingsChanged();

	void
	RendererTypeChanged( int aRendererType );

	void
	ColorTransformTypeChanged( int aColorTransform );

	void
	MouseInfoUpdate( const QString &aInfo );
	/*void
	SetImage( M4D::Imaging::AImage::Ptr image )
	{ _image = image; }*/

protected:
	void	
	initializeGL ();

	void	
	initializeOverlayGL ();

	void	
	paintGL ();

	void	
	paintOverlayGL ();

	void	
	resizeGL ( int width, int height );

	void	
	resizeOverlayGL ( int width, int height );

	void
	mouseMoveEvent ( QMouseEvent * event );

	void	
	mouseDoubleClickEvent ( QMouseEvent * event );

	void
	mousePressEvent ( QMouseEvent * event );

	void
	mouseReleaseEvent ( QMouseEvent * event );

	void
	wheelEvent ( QWheelEvent * event );


	bool
	IsDataPrepared();

	bool
	PrepareData();

	QString
	GetVoxelInfo( Vector3i aDataCoords );

	enum {rmONE_DATASET}	_renderingMode;

	GLTextureImage::Ptr	_textureData;


	Vector< float, 3 > 			_regionRealMin;
	Vector< float, 3 >			_regionRealMax;
	Vector< float, 3 >			_elementExtents;
	Vector< int32, 3 > 			_regionMin;
	Vector< int32, 3 >			_regionMax;

	TransferFunctionBuffer1D::Ptr 		mTFunctionBuffer;
	GLTransferFunctionBuffer1D::Ptr 	mTransferFunctionTexture;

	 bool					_prepared;
	
	//RenderingThread				mRenderingThread;
	//QGLContext 				*mOtherContext;


	FrameBufferObject			mFrameBufferObject;

	//QGLWidget				*mDummyGLWidget;

	bool					mSaveFile; //TODO handle differently
	bool					mSaveCycle; //TODO handle differently


	int mRendererType;

	M4D::GUI::Renderer::SliceRenderer	mSliceRenderer;
	M4D::GUI::Renderer::SliceRenderer::RenderingConfiguration mSliceRenderConfig;

	M4D::GUI::Renderer::VolumeRenderer	mVolumeRenderer;
	M4D::GUI::Renderer::VolumeRenderer::RenderingConfiguration mVolumeRenderConfig;

	//QStateMachine	mStateMachine;
	std::map< int, IUserEvents::Ptr>	mUserEventHandlers;
	IUserEvents				*mCurrentEventHandler;
private:

};

} /*namespace Viewer*/
} /*namespace GUI*/
} /*namespace M4D*/



#endif /*BASIC_SLICE_VIEWER_H*/



#endif /*USE_CG*/
