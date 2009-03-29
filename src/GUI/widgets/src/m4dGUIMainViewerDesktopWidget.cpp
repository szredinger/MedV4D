/**
 *  @ingroup gui
 *  @file m4dGUIMainViewerDesktopWidget.cpp
 *  @brief some brief
 */
#include "GUI/widgets/m4dGUIMainViewerDesktopWidget.h"

#include "GUI/widgets/m4dGUIMainWindow.h"

using namespace M4D::Imaging;
using namespace M4D::Viewer;

using namespace std;


namespace M4D {
namespace GUI {

m4dGUIMainViewerDesktopWidget::m4dGUIMainViewerDesktopWidget ( const unsigned rows, const unsigned columns, 
                                                              QWidget *parent )
  : QWidget( parent ), defaultConnection( NULL )
{
  setDesktopLayout( rows, columns );

  selectedViewer = viewers[0];
  viewers[0]->viewerWidget->slotSetSelected( true );
}


void m4dGUIMainViewerDesktopWidget::setDefaultConnection ( ConnectionInterface *conn )
{
	defaultConnection = conn;
}


void m4dGUIMainViewerDesktopWidget::setConnectionForAll ( ConnectionInterface *conn )
{
  for ( unsigned i = 0; i < viewers.size(); ++i ) {
		viewers[i]->viewerWidget->setInputPort( conn );
	}
}


void m4dGUIMainViewerDesktopWidget::setViewerEventHandlerForSelected ( m4dGUIViewerEventHandlerInterface *eventHandler )
{
  selectedViewer->viewerWidget->setViewerEventHandler( eventHandler );
}


void m4dGUIMainViewerDesktopWidget::setViewerEventHandlerForAll ( m4dGUIViewerEventHandlerInterface *eventHandler )
{
  for ( unsigned i = 0; i < viewers.size(); ++i ) {
		viewers[i]->viewerWidget->setViewerEventHandler( eventHandler );
	}
}


void m4dGUIMainViewerDesktopWidget::replaceSelectedViewerWidget ( ViewerType type, 
                                                                  m4dGUIAbstractViewerWidget *replacedViewer ) 
{
  ConnectionInterface *conn = replacedViewer->getInputPort();
  unsigned idx = replacedViewer->getIndex();
  list< string > leftOverlayInfo  = replacedViewer->getLeftSideTextData();
  list< string > rightOverlayInfo = replacedViewer->getRightSideTextData();

  m4dGUIAbstractViewerWidget *widget = 0;

  switch ( type )
  {
    case SLICE_VIEWER:
      widget = new m4dGUISliceViewerWidget( conn, idx );
      break;

    case VTK_VIEWER:
      widget = new m4dGUIVtkViewerWidget( conn, idx );
      break;

    default:
      widget = new m4dGUISliceViewerWidget( conn, idx );
  }

  widget->setLeftSideTextData( leftOverlayInfo ); 
  widget->setRightSideTextData( rightOverlayInfo ); 

  connect( widget, SIGNAL(signalSetSelected( unsigned, bool )), this, SLOT(selectedChanged( unsigned )) );
  widget->slotSetSelected( true );

  selectedViewer->type = type;
  selectedViewer->viewerWidget = widget;
  selectedViewer->checkedLeftButtonTool = selectedViewer->checkedRightButtonTool = ACTION_EMPTY;

  QLayoutItem *li = layout()->itemAt( 0 );
  QSplitter *mainSplitter = (QSplitter *)(li->widget());
  QSplitter *innerSplitter = (QSplitter *)(mainSplitter->widget( idx / layoutColumns ));

  QWidget *resizedWidget = (*widget)();
  resizedWidget->resize( resizedWidget->sizeHint() );

  QByteArray state = innerSplitter->saveState();

  // delete old - directly before inserting the new one 
  delete replacedViewer;
  innerSplitter->insertWidget( idx % layoutColumns, resizedWidget );

  innerSplitter->restoreState( state );
}


void m4dGUIMainViewerDesktopWidget::addSource ( ConnectionInterface *conn, 
                                                m4dGUIViewerEventHandlerInterface *viewerEventHandler )
{
  sources.push_back( Source( conn, viewerEventHandler ) );
}


void m4dGUIMainViewerDesktopWidget::setDesktopLayout( const unsigned rows, const unsigned columns )
{
  unsigned newSize = rows * columns;
  unsigned viewersSize = viewers.size();
  int difference = newSize - viewersSize;

  if ( difference > 0 )
  {
    for ( int i = 0; i < difference; i++ ) 
    {
      Viewer *viewer = new Viewer;

      m4dGUIAbstractViewerWidget *widget = new m4dGUISliceViewerWidget( viewersSize + i );
	    if( defaultConnection ) {
		    widget->setInputPort( defaultConnection );
	    }

      connect( widget, SIGNAL(signalSetSelected( unsigned, bool )), this, SLOT(selectedChanged( unsigned )) );
      
      viewer->viewerWidget = widget;
      viewer->type = SLICE_VIEWER;
      viewer->checkedLeftButtonTool = viewer->checkedRightButtonTool = ACTION_EMPTY;
      viewer->sourceIdx = 0;

      viewers.push_back( viewer );
    }
  }
  else
  {
    if ( !viewers[newSize - 1]->viewerWidget->getSelected() ) {
      viewers[newSize - 1]->viewerWidget->slotSetSelected( true );
    }
    for ( unsigned i = newSize; i < viewersSize; i++ ) {
      delete viewers[i];
    }
    viewers.resize( newSize );
  }

  QGridLayout *mainLayout = new QGridLayout;

  QSplitter *mainSplitter = new QSplitter();
  mainSplitter->setOrientation( Qt::Vertical );

  for ( unsigned i = 0; i < rows; i++ )
  {
    QSplitter *splitter = new QSplitter();
    for ( unsigned j = 0; j < columns; j++ )
    {   
      QWidget *widget = (*viewers[i * columns + j]->viewerWidget)();
      widget->resize( widget->sizeHint() );
      splitter->addWidget( widget );
    }
    mainSplitter->addWidget( splitter );
  }

  mainLayout->addWidget( mainSplitter );

  delete layout();
  setLayout( mainLayout );

  layoutRows = rows;
  layoutColumns = columns;
}


void m4dGUIMainViewerDesktopWidget::selectedChanged ( unsigned index )
{
  prevSelectedViewer = selectedViewer;
  selectedViewer = viewers[index];

  if ( prevSelectedViewer != selectedViewer ) {
    prevSelectedViewer->viewerWidget->slotSetSelected( false );
  }

  emit propagateFeatures( prevSelectedViewer->viewerWidget ); 
}


void m4dGUIMainViewerDesktopWidget::sourceSelected ( int index )
{ 
  selectedViewer->sourceIdx = index;

  selectedViewer->viewerWidget->InputPort()[0].UnPlug();
  sources[index].conn->ConnectConsumer( selectedViewer->viewerWidget->InputPort()[0] );

  selectedViewer->viewerWidget->setViewerEventHandler( sources[index].hnd );
}

} // namespace GUI
} // namespace M4D