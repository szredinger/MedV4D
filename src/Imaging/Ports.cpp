#include <algorithm>

#include "Thread.h"
#include "Functors.h"
#include "Imaging/Ports.h"

namespace M4D
{
namespace Imaging
{

/* Port::PortID */ uint64
Port::GenerateUniqueID()
{
	static /* PortID */ uint64 lastID = 0;
	static Multithreading::Mutex genMutex;

	{	//We must synchronize to avoid multiple generation of one ID.
		Multithreading::ScopedLock lock( genMutex );
		return ++lastID;
	}
}
//******************************************************************************
	
void
Port::ReceiveMessage( 
	PipelineMessage::Ptr 			msg, 
	PipelineMessage::MessageSendStyle 	sendStyle, 
	FlowDirection				direction
	)
{
	//TODO handle special situations - messages for port, etc.
	if( _msgReceiver ) {
		_msgReceiver->ReceiveMessage( msg, sendStyle, direction );
	}
}
//******************************************************************************

bool 
InputPort::GetDatasetReadLock()
{

}

void 
InputPort::ReleaseDatasetLock()
{

}

//******************************************************************************

bool 
OutputPort::GetDatasetWriteLock()
{

}

void
OutputPort::ReleaseDatasetLock()
{

}

//******************************************************************************

const AbstractImage &
InputPortAbstractImage
::GetAbstractImage()const
{
	if( !this->IsPlugged() ) {
		throw EDisconnected( this->GetID() );
	}
	return _abstractImageConnection->GetAbstractImageReadOnly();
}


void
InputPortAbstractImage
::Plug( ConnectionInterface & connection )
{
	AbstractImageConnection *conn = 
		dynamic_cast< AbstractImageConnection * >( &connection );
	if( conn ) {
		_abstractImageConnection = conn;
	} else {
		throw Port::EConnectionTypeMismatch();
	}
}

/*void
InputPortImageFilter
::PlugTyped( AbstractImageConnection & connection )
{
	_abstractImageConnection = &connection;
	//TODO
}*/

void
InputPortAbstractImage
::UnPlug()
{
	_abstractImageConnection = NULL;//TODO
}

void
InputPortAbstractImage
::SendMessage( 
		PipelineMessage::Ptr 			msg, 
		PipelineMessage::MessageSendStyle 	sendStyle 
		)
{
	if( this->IsPlugged() ) {
		msg->senderID = this->GetID();
		_abstractImageConnection->RouteMessage( msg, sendStyle, FD_AGAINST_FLOW );
		
	}
	//TODO
}


//******************************************************************************

InputPortList::~InputPortList()
{
	std::for_each( 
		_ports.begin(), 
		_ports.end(), 
		Functors::Deletor< InputPort* >() 
		);
}

size_t
InputPortList::AddPort( InputPort* port )
{
	if( port == NULL ) {
		//TODO - throw exception
		return static_cast< size_t >( -1 );
	}
	_ports.push_back( port );
	port->SetReceiver( _msgReceiver );
	return _size++;
}

InputPort &
InputPortList::GetPort( size_t idx )const
{
	//TODO - check bounds - throw exception
	return *_ports[ idx ];
}

void
InputPortList::SendMessage( 
		PipelineMessage::Ptr 			msg, 
		PipelineMessage::MessageSendStyle 	sendStyle
		)
{
	std::for_each( 
		_ports.begin(), 
		_ports.end(), 
		MessageSenderFunctor< InputPort* >( msg, sendStyle )
		);
}

OutputPortList::~OutputPortList()
{
	std::for_each( 
		_ports.begin(), 
		_ports.end(), 
		Functors::Deletor< OutputPort* >() 
		);
}

size_t
OutputPortList::AddPort( OutputPort* port )
{
	if( port == NULL ) {
		//TODO - throw exception
		return static_cast< size_t >( -1 );
	}
	_ports.push_back( port );
	port->SetReceiver( _msgReceiver );
	return _size++;
}

OutputPort &
OutputPortList::GetPort( size_t idx )const
{
	//TODO - check bounds - throw exception
	return *_ports[ idx ];
}

void
OutputPortList::SendMessage( 
		PipelineMessage::Ptr 			msg, 
		PipelineMessage::MessageSendStyle 	sendStyle
		)
{
	std::for_each( 
		_ports.begin(), 
		_ports.end(), 
		MessageSenderFunctor< OutputPort* >( msg, sendStyle )
		);
}

}/*namespace Imaging*/
}/*namespace M4D*/
