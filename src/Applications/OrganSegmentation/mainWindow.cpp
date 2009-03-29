#include "mainWindow.h"
#include "SettingsBox.h"
#include "Imaging/PipelineMessages.h"

#include "MainManager.h"
#include "ManualSegmentationManager.h"
#include "KidneySegmentationManager.h"

using namespace std;
using namespace M4D::Imaging;


mainWindow::mainWindow ()
  : m4dGUIMainWindow2( APPLICATION_NAME, ORGANIZATION_NAME )//, _inConnection( NULL ), _outConnection( NULL )
{
	Q_INIT_RESOURCE( mainWindow ); 


	currentViewerDesktop->setDefaultConnection( MainManager::Instance().GetInputConnection() );
	currentViewerDesktop->setConnectionForAll( currentViewerDesktop->getDefaultConnection() );

	// add your own settings widgets
	_settings = new SettingsBox( this );
	QObject::connect( _settings, SIGNAL( SetSegmentationSignal( uint32 )),
		this, SLOT(SetSegmentationSlot( uint32 )) );

	addDockWindow( "Organ Segmentation", _settings );

	_segmentationWidget = new SegmentationWidget();
	addDesktopWidget( _segmentationWidget );
}


void 
mainWindow::process ( M4D::Dicom::DicomObjSetPtr dicomObjSet )
{
	MainManager::Instance().InitInput( dicomObjSet );

	_settings->SetToDefault();

	//mainViewerDesktop->setConnectionForAll( mainViewerDesktop->getDefaultConnection() );

	/*AbstractImage::Ptr inputImage = ImageFactory::CreateImageFromDICOM( dicomObjSet );

	try {
		_inConnection->PutImage( inputImage );

		_convertor->Execute();

		mainViewerDesktop->getSelectedViewerWidget()->InputPort()[0].UnPlug();
		_inConnection->ConnectConsumer( mainViewerDesktop->getSelectedViewerWidget()->InputPort()[0] );

		_settings->SetEnabledExecButton( true );
	} 
	catch( ... ) {
		QMessageBox::critical( this, tr( "Exception" ), tr( "Some exception" ) );
	}*/

}

void
mainWindow::SetSegmentationSlot( uint32 segType )
{
	switch( segType ) {
	case stMANUAL:
		_segmentationWidget->Activate( ManualSegmentationManager::Instance().GetInputConnection(), ManualSegmentationManager::Instance().GetSpecialState() );

		stackWidget->setCurrentWidget( _segmentationWidget );
		break;
	case stKIDNEYS:
		_segmentationWidget->Activate( KidneySegmentationManager::Instance().GetInputConnection(), KidneySegmentationManager::Instance().GetSpecialState() );

		stackWidget->setCurrentWidget( _segmentationWidget );
		break;
	case stDefault:
		stackWidget->setCurrentIndex( 0 );
		break;
	default:
		ASSERT( false );
		break;
	}
}


