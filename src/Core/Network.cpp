/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core.h"
#include "vapor/net/Network.h"
#include "vapor/net/Message.h"
#include <zmq.h>

namespace vapor {

//-----------------------------------//

Network::~Network()
{
	shutdown();
}

//-----------------------------------//

bool Network::init( int numThreads )
{
	context = zmq_init( numThreads );

	if(!context)
	{
		error("Error initializing ZeroMQ");
		return false;
	}

	int major, minor, patch;
	zmq_version (&major, &minor, &patch);

	Log::info( 
		"Initialized ZeroMQ %d.%d.%d with %d I/O thread(s)",
		major, minor, patch, numThreads );

	return true;
}

//-----------------------------------//

bool Network::createServerSocket(const std::string& uri)
{
	assert( socket != nullptr );

	socket = zmq_socket( context, ZMQ_XREP );
	int status = zmq_bind( socket, uri.c_str() );

	if(status != 0)
	{
		error("Error creating server socket");
		return false;
	}

	Log::info("Created a server socket at '%s'", uri.c_str());
	
	return true;
}

//-----------------------------------//

bool Network::createClientSocket(const std::string& uri)
{
	assert( socket != nullptr );
	
	socket = zmq_socket( context, ZMQ_REQ );
	int status = zmq_connect( socket, uri.c_str() );

	if(status != 0)
	{
		error("Error creating client socket");
		return false;
	}

	Log::info("Created a client socket at '%s'", uri.c_str());

	return true;
}

//-----------------------------------//

void Network::shutdown()
{
	if( !context )
		return;

	int status = zmq_term(context);
	
	if(status != 0)
		error("Error terminating ZeroMQ");

	context = nullptr;
}

//-----------------------------------//

void Network::waitMessages()
{
	int ret;
	
	int64 more;
	size_t more_size = sizeof(more);
	
	while(true)
	{
		MessagePtr message = new Message();
		message->init();

		// Wait for next request from client
		ret = zmq_recv(socket, message->getBuffer(), 0);

		if( ret != 0 )
			error("Error receiving message");

		// Determine if more message parts are to follow
		ret = zmq_getsockopt(socket, ZMQ_RCVMORE, &more, &more_size);
		
		if( ret != 0 )
		{
			Log::info("Handle multi-part messages");
		}

		messages.push(message);
	}
}

//-----------------------------------//

void Network::dispatchMessages()
{
	MessagePtr msg;
	
	while(true)
	{
		messages.wait_and_pop(msg);

		MessageType::Enum type = msg->getMessageType();
		MessageHandlersMap::iterator it = messageHandlers.find(type);
		
		if( it == messageHandlers.end() )
		{
			Log::warn("No message handler for type: %d", type);
			continue;
		}

		MessageHandler* handler = (*it).second;
		assert(handler != nullptr);
		
		handler->handleMessage(msg);
	}
}

//-----------------------------------//

void Network::sendMessage( const MessagePtr& message )
{
	assert( context && socket );
	
	int status = zmq_send(socket, message->getBuffer(), 0);

	if( status != 0 )
		error("Error sending message");
}

//-----------------------------------//

void Network::error(const char* str)
{
	int err = zmq_errno();
	Log::error("%s: %s", str, zmq_strerror(err));
}

//-----------------------------------//

} // end namespace