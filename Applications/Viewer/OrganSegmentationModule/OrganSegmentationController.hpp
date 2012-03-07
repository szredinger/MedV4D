#ifndef ORGAN_SEGMENTATION_CONTROLLER_H
#define ORGAN_SEGMENTATION_CONTROLLER_H

#include <QtCore>
#include "MedV4D/GUI/widgets/GeneralViewer.h"
#include "MedV4D/GUI/managers/ApplicationManager.h"
#include "MedV4D/GUI/utils/ProxyViewerController.h"
#include "MedV4D/GUI/utils/QtModelViewTools.h"
#include "MedV4D/GUI/utils/DrawingMouseController.h"
#include "MedV4D/GUI/utils/PrimitiveCreationEventController.h"
#include "MedV4D/GUI/managers/OpenGLManager.h"

#include "MedV4D/Imaging/painting/Paint.h"
#include "MedV4D/Imaging/CanonicalProbModel.h"

class MaskDrawingMouseController: public ADrawingMouseController
{
public:
	typedef boost::shared_ptr<MaskDrawingMouseController> Ptr;
	MaskDrawingMouseController( M4D::Imaging::Mask3D::Ptr aMask ): mMask( aMask )
	{ ASSERT( mMask ); }
protected:	
	void
	drawStep( const Vector3f &aStart, const Vector3f &aEnd )
	{
		float width = 10.0f;
		Vector3f offset( width, width, width );
		Vector3f minimum = M4D::minVect<float,3>( M4D::minVect<float,3>( aStart - offset, aStart + offset ), M4D::minVect<float,3>( aEnd - offset, aEnd + offset ) );
		Vector3f maximum = M4D::maxVect<float,3>( M4D::maxVect<float,3>( aStart - offset, aStart + offset ), M4D::maxVect<float,3>( aEnd - offset, aEnd + offset ) );
		
		Vector3i c1 = M4D::maxVect<int,3>( mMask->GetElementCoordsFromWorldCoords( minimum ), mMask->GetMinimum() );
		Vector3i c2 = M4D::minVect<int,3>( mMask->GetElementCoordsFromWorldCoords( maximum ), mMask->GetMaximum() );
		M4D::Imaging::WriterBBoxInterface & mod = mMask->SetDirtyBBox( c1, c2 );
		LOG( "EditedA : " << c1 << " => " << c2 );
		try {
			M4D::Imaging::painting::drawRectangleAlongLine( *mMask, 255, aStart, aEnd, 10, Vector3f( 0.0f, 0.0f, 1.0f ) );
		} catch (...){
			D_PRINT( "drawStep exception" );
		}
		//M4D::Imaging::WriterBBoxInterface & mod = mMask->SetWholeDirtyBBox();
		mod.SetModified();
		OpenGLManager::getInstance()->getTextureFromImage(*mMask);
		/*D_PRINT( "DRAW " << aStart );
		Vector3f diff = 0.1f*(aEnd-aStart);
		for( size_t i = 0; i <= 10; ++i ) {
			try {
				mMask->GetElementWorldCoords( aStart + float(i) * diff ) = 255;
			} catch (...){
				D_PRINT( "drawStep exception" );
			}
		}*/
	}
	
	M4D::Imaging::Mask3D::Ptr mMask;
};

class OrganSegmentationController: public ModeViewerController, public M4D::GUI::Viewer::RenderingExtension
{
	Q_OBJECT;
public:
	enum MeasurementMode {
		mmNONE,
		mmHUMERAL_HEAD,
		mmPROXIMAL_SHAFT,

		mmSENTINEL //use for valid interval testing 
	};

	typedef QList<QAction *> QActionList;
	typedef boost::shared_ptr< OrganSegmentationController > Ptr;
	typedef M4D::GUI::Viewer::ViewerController ControllerPredecessor;
	typedef M4D::GUI::Viewer::RenderingExtension RenderingExtPredecessor;

	OrganSegmentationController();
	~OrganSegmentationController();

	void
	activated()
	{

	}

	void
	deactivated()
	{
		//std::for_each( mChosenToolActions.begin(), mChosenToolActions.end(), boost::bind( &QAction::setChecked, _1, false ) );
	}

	void
	setModeId( M4D::Common::IDNumber aId )
	{
		mModeId = aId;
	}

	bool
	mouseMoveEvent ( M4D::GUI::Viewer::BaseViewerState::Ptr aViewerState, const M4D::GUI::Viewer::MouseEventInfo &aEventInfo );

	bool	
	mouseDoubleClickEvent ( M4D::GUI::Viewer::BaseViewerState::Ptr aViewerState, const M4D::GUI::Viewer::MouseEventInfo &aEventInfo );

	bool
	mousePressEvent ( M4D::GUI::Viewer::BaseViewerState::Ptr aViewerState, const M4D::GUI::Viewer::MouseEventInfo &aEventInfo );

	bool
	mouseReleaseEvent ( M4D::GUI::Viewer::BaseViewerState::Ptr aViewerState, const M4D::GUI::Viewer::MouseEventInfo &aEventInfo );

	bool
	wheelEvent ( M4D::GUI::Viewer::BaseViewerState::Ptr aViewerState, QWheelEvent * event );

	unsigned
	getAvailableViewTypes()const;

	void
	render2DAlignedSlices( int32 aSliceIdx, Vector2f aInterval, CartesianPlanes aPlane );

	void
	preRender3D();

	void
	postRender3D();

	void
	render3D();

	M4D::Imaging::Mask3D::Ptr	mMask;
signals:
	void
	updateRequest();

public slots:
	void
	toggleMaskDrawing( bool aToggle );

protected:

public:
	Qt::MouseButton	mVectorEditorInteractionButton;

	bool mOverlay;

	M4D::Common::IDNumber mModeId;
	
	MaskDrawingMouseController::Ptr mMaskDrawingController;
};



#endif /*ORGAN_SEGMENTATION_CONTROLLER_H*/