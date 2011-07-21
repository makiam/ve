/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Server/API.h"
#include "Server/Server.h"
#include "Server/Settings.h"
#include "Server/ServerPlugin.h"

#include "Core/Event.h"
#include "Core/Concurrency.h"
#include "Core/Utilities.h"

#include "Network/Network.h"
#include "Network/Host.h"
#include "Network/Peer.h"
#include "Network/Dispatcher.h"

#include <iostream>

NAMESPACE_SERVER_BEGIN

//-----------------------------------//

static Allocator* gs_AllocatorServer = nullptr;

void ServerInitialize()
{
	gs_AllocatorServer = AllocatorCreateHeap(AllocatorGetHeap());
	AllocatorSetGroup(gs_AllocatorServer, "Server");
}

void ServerDeinitialize()
{
	AllocatorDestroy(gs_AllocatorServer);
}

Allocator* AllocatorGetServer()
{
	return gs_AllocatorServer;
}

static Server* gs_ServerInstance = nullptr;
Server* GetServer() { return gs_ServerInstance; }

//-----------------------------------//

Server::Server()
	: tasks(nullptr)
	, networkThread(nullptr)
	, dispatcher(nullptr)
{
	gs_ServerInstance = this;
}

//-----------------------------------//

bool Server::init()
{
	NetworkInitialize();

#if 0
	Log::info("Created %d processing task(s)", Settings::NumTasksProcess);
#endif

	networkThread = ThreadCreate(AllocatorGetServer());
	
	host = Allocate(HostServer, AllocatorGetServer());
	host->createSocket("", Settings::HostPort);
	host->onClientConnected.Connect(this, &Server::handleClientConnect);
	host->onClientDisconnected.Connect(this, &Server::handleClientDisconnect);

	dispatcher = Allocate(Dispatcher, AllocatorGetServer());
	dispatcher->initServer(host);
	dispatcher->initPlugins(ReflectionGetType(MessagePlugin));

	return true;
}

//-----------------------------------//

void Server::shutdown()
{
	Deallocate(dispatcher);
	Deallocate(host);
	ThreadDestroy(networkThread);
	NetworkDeinitialize();
}

//-----------------------------------//

static void ProcessMessagesThread(Thread* thread, void* data)
{
	Host* host = (Host*) data;
	
	LogInfo("Networking thread will now start listening for messages...");

	while(thread->IsRunning)
	{
		host->processEvents(100);
	}
}

//-----------------------------------//

void Server::run()
{
	ThreadStart(networkThread, ProcessMessagesThread, host);
	ThreadSetName(networkThread, "Networking");

	if(!networkThread)
	{
		LogError("Error creating networking thread");
		return;
	}

	while(true)
	{
		SystemSleep(0);

		String input;
		std::getline(std::cin, input);

		if(StringCompareInsensitive(input, "Quit") == 0)
			break;
	}
}

//-----------------------------------//

void Server::handleClientConnect(const PeerPtr& networkPeer)
{
	String hostname = networkPeer->getHostName().c_str();
	String ip = networkPeer->getHostIP().c_str();

	LogInfo("Client connected: %s (IP: %s)", hostname.c_str(), ip.c_str());
}

//-----------------------------------//

void Server::handleClientDisconnect(const PeerPtr& networkPeer)
{
	String hostname = networkPeer->getHostName().c_str();
	LogInfo("Client disconnected: %s", hostname.c_str());
}


//-----------------------------------//

NAMESPACE_SERVER_END