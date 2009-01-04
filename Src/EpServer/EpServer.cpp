// EpServer.cpp : Defines the entry point for the console application.
//

#include "EpServerPCH.h"
#include "ClientPool.h"
#include "EpReplicaManagerConnection.h"
#include "EpUser.h"
#include "UnitBase.h"

RakNet::RPC3						g_rpc3Inst;
ClientPool							g_clientPool;
RakNet::ReplicaManager2				g_replicaManager;		// The system that performs most of our functionality for this demo
EpReplicaManagerConnectionFactory	g_connectionFactory;	// Instance of the class that creates the object we use to represent connections
NetworkIDManager					g_networkIDManager;
RakPeerInterface*					g_server;

// We copy this from Multiplayer.cpp to keep things all in one file for this example
unsigned char GetPacketIdentifier(Packet *p);

// These functions are called from EpCommonLibrary
NetworkIDManager& GetNetworkIdManager() { return g_networkIDManager; }
RakPeerInterface* GetRakPeer() { return g_server; }
bool QueryIsNetworkServer() { return true; }


void AddToCurrentWorld( UnitBase* unitBase )
{
	// Client-exclusive function
	throw std::runtime_error( "Should not be called on server-side." );
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	UnitBase::myUnit = 0;
	EpUser::myUser = 0;
	
	g_networkIDManager.SetIsNetworkIDAuthority(true);
	g_rpc3Inst.SetNetworkIDManager(&g_networkIDManager);

	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	g_server = RakNetworkFactory::GetRakPeerInterface();

	g_server->SetNetworkIDManager( &g_networkIDManager );

	RakNetStatistics* rss = 0;
	unsigned i = g_server->GetNumberOfAddresses();
	g_server->SetIncomingPassword( "Rumpelstiltskin", (int)strlen( "Rumpelstiltskin" ) );

	// Holds packets
	Packet* p = 0;

	// GetPacketIdentifier returns this
	unsigned char packetIdentifier;

	// Record the first client that connects to us so we can pass it to the ping function
	//SystemAddress clientID = UNASSIGNED_SYSTEM_ADDRESS;

	// Holds user data
	const unsigned serverPort = 10000;

	puts( "----------------" );
	puts( "|   EpServer   |" );
	puts( "----------------" );

	printf(" - This Server Port: %d\n", serverPort);

	puts( "Starting server." );
	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	SocketDescriptor socketDescriptor( serverPort, 0 );
	const unsigned short maxConnections = 32;
	const int threadSleepTimer = 30;
	bool b = g_server->Startup( maxConnections, threadSleepTimer, &socketDescriptor, 1 );
	g_server->AttachPlugin( &g_replicaManager );
	// Just test this
	g_replicaManager.SetAutoAddNewConnections( false );
	// Register our custom connection factory
	g_replicaManager.SetConnectionFactory( &g_connectionFactory );

	g_server->SetMaximumIncomingConnections( 4 );
	if ( b )
		puts( "Server started, waiting for connections." );
	else
	{ 
		puts( "Server failed to start.  Terminating." );
		exit( 1 );
	}
	g_server->SetOccasionalPing( true );

	printf( "This Server IP: %s\n", g_server->GetLocalIP( 0 ) );
	puts(" - 'quit' to quit.");
	puts(" - 'stat' to show stats.");
	puts(" - 'ban' to ban an IP from connecting.");
	puts(" - 'kick' to kick the first connected player.");
	puts(" - 'user' to query online users.");
	puts("-------------");
	puts("Type to talk." );

	g_server->AttachPlugin(&g_rpc3Inst);

	// Here I use the string table class to efficiently send strings I know in advance.
	// The encoding is used in in Replica2::SerializeConstruct
	// The decoding is used in in Connection_RM2::Construct
	// The stringTable class will also send strings that weren't registered but this just falls back to the stringCompressor and wastes 1 extra bit on top of that
	// 2nd parameter of false means a static string so it's not necessary to copy it
	RakNet::StringTable::Instance()->AddString("UnitBase", false);
	RakNet::StringTable::Instance()->AddString("EpUser", false);

	EpUser* masterUser = new EpUser;
	// In order to use any networked member functions of Replica2, you must first call SetReplicaManager
	masterUser->SetReplicaManager( &g_replicaManager );
	// Store which address this user is associated with
	masterUser->setSystemAddress(g_server->GetSystemAddressFromIndex(0));
	// Tell the user to automatically serialize our data members every 100 milliseconds (if changed)
	// This way if we change the system address or the UnitBase* we don't have to call user->BroadcastSerialize();
	masterUser->AddAutoSerializeTimer( 100 );
	// Send out this new user to all systems. Unlike the old system (ReplicaManager) all sends are done immediately.
	masterUser->BroadcastConstruction();

	char message[2048];

	// Loop for input
	while ( 1 )
	{

		// This sleep keeps RakNet responsive
		RakSleep( 30 );

#ifdef _WIN32
		if ( _kbhit() )
		{
			// Notice what is not here: something to keep our network running.  It's
			// fine to block on gets or anything we want
			// Because the network engine was painstakingly written using threads.
			gets_s( message, 2048 );

			if ( strcmp( message, "quit" ) == 0 || strcmp( message, "q" ) == 0 )
			{
				puts( "Quitting." );
				break;
			}

			if ( strcmp( message, "stat" ) == 0 )
			{
				rss = g_server->GetStatistics( g_server->GetSystemAddressFromIndex( 0 ) );
				StatisticsToString( rss, message, 2 );
				printf( "%s", message );
				printf( "Ping %i\n", g_server->GetAveragePing( g_server->GetSystemAddressFromIndex( 0 ) ) );

				continue;
			}


			if ( strcmp( message, "kick" ) == 0 )
			{
				//g_server->CloseConnection( clientID, true, 0 );
				continue;
			}

			if ( strcmp( message, "create" ) == 0 )
			{
				RakNet::RakString command( "createEnemy2 {12 60}" );
				g_rpc3Inst.CallC( "EpRpcDoScript", command );

				continue;
			}

			if ( strcmp( message, "move" ) == 0 )
			{
				NetworkID nid;
				nid.localSystemAddress = 100;
				int tilePosX = 0, tilePosY = 0;
				g_rpc3Inst.CallCPP( "&Unit::setTilePosRpc", nid, tilePosX, tilePosY );
			}

			if ( strcmp( message, "user" ) == 0 )
			{
				unsigned i;
				const unsigned unitCount = UnitBase::units.Size();
				printf("%i Units:\n", unitCount);
				for (i = 0; i < unitCount; i++)
				{
					printf("%i. ptr=%p name=%s isCloaked=%i owner=%p Position=%s\n",
						i+1,
						UnitBase::units[i],
						UnitBase::units[i]->getRepName().C_String(),
						UnitBase::units[i]->isHidden(),
						UnitBase::units[i]->getOwner(),
						UnitBase::units[i]->getPositionAsString() );
				}

				const unsigned userCount = EpUser::users.Size();
				printf("%i Users:\n", userCount);
				for (i = 0; i < userCount; i++)
				{
					printf("%i. ptr=%p systemAddress=%s unit=%p\n",
						i+1,
						EpUser::users[i],
						EpUser::users[i]->getSystemAddress().ToString(),
						EpUser::users[i]->getUnit());
				}
			}


			if ( strcmp(message, "ban") == 0 )
			{
				printf( "Enter IP to ban.  You can use * as a wildcard\n" );
				gets_s( message, 2048 );
				g_server->AddToBanList( message );
				printf( "IP %s added to ban list.\n", message );

				continue;
			}


			// Message now holds what we want to broadcast
			char message2[2048];
			// Append Server: to the message so clients know that it ORIGINATED from the server
			// All messages to all clients come from the server either directly or by being
			// relayed from other clients
			message2[0] = 0;
			strcpy_s( message2, 2048, "Server: " );
			strcat_s( message2, 2048, message );

			// message2 is the data to send
			// strlen(message2)+1 is to send the null terminator
			// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
			// RELIABLE_ORDERED means make sure the message arrives in the right order
			// We arbitrarily pick 0 for the ordering stream
			// UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast
			// true means broadcast the message to everyone connected
			g_server->Send( message2, (const int) strlen(message2)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true );
		}
#endif

		// Get a packet from either the server or the client

		p = g_server->Receive();

		if ( p == 0 )
			continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		packetIdentifier = GetPacketIdentifier( p );

		// Check if this is a network message packet
		switch ( packetIdentifier )
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf( "ID_DISCONNECTION_NOTIFICATION\n" );
			g_clientPool.removeClient( p->systemAddress );
			printf( "Current Clients: %d\n", g_clientPool.getClientCount() );
			EpUser::DeleteUserByAddress( p->systemAddress );
			break;


		case ID_NEW_INCOMING_CONNECTION:
			{
				// Somebody connected.  We have their IP now
				printf( "ID_NEW_INCOMING_CONNECTION from %s\n", p->systemAddress.ToString() );
				//clientID = p->systemAddress; // Record the player ID of the client
				g_clientPool.addClient( p->systemAddress );
				printf( "Current Clients: %d\n", g_clientPool.getClientCount() );
				
				// RPC3 functionality test
				int a = 2008;
				g_rpc3Inst.CallC( "PrintHelloWorld", a );

				// Necessary with SetAutoAddNewConnections(false);
				g_replicaManager.AddNewConnection( p->systemAddress );
				// The server should create a User class for each new connection.
				EpUser* newUser = new EpUser;
				// In order to use any networked member functions of Replica2, you must first call SetReplicaManager
				newUser->SetReplicaManager( &g_replicaManager );
				// Store which address this user is associated with
				newUser->setSystemAddress(p->systemAddress);
				// Tell the user to automatically serialize our data members every 100 milliseconds (if changed)
				// This way if we change the system address or the UnitBase* we don't have to call user->BroadcastSerialize();
				newUser->AddAutoSerializeTimer( 100 );
				// Send out this new user to all systems. Unlike the old system (ReplicaManager) all sends are done immediately.
				newUser->BroadcastConstruction();

				/*
				UnitBase* newUnitBase = new UnitBase(UT_UNITBASE);
				newUnitBase->setOwner(masterUser);
				newUnitBase->setTilePos( 50, 50 );
				newUnitBase->setTileBufferPos( 50, 50 );
				newUnitBase->SetReplicaManager( &g_replicaManager );
				newUnitBase->AddAutoSerializeTimer( 100 );
				newUnitBase->BroadcastConstruction();
				//newUnitBase->setArnMeshName( "GwengYiModel" );
				*/
			}
			
			
			break;

		case ID_MODIFIED_PACKET:
			// Cheater!
			printf( "ID_MODIFIED_PACKET\n" );
			DebugBreak();
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf( "ID_CONNECTION_LOST\n" );
			g_clientPool.removeClient( p->systemAddress );
			EpUser::DeleteUserByAddress( p->systemAddress );
			break;
		case ID_RPC_REMOTE_ERROR:
			{
				// Recipient system returned an error
				switch (p->data[1])
				{
				case RakNet::RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE:
					printf("RPC_ERROR_NETWORK_ID_MANAGER_UNAVAILABLE\n");
					break;
				case RakNet::RPC_ERROR_OBJECT_DOES_NOT_EXIST:
					printf("RPC_ERROR_OBJECT_DOES_NOT_EXIST\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE:
					printf("RPC_ERROR_FUNCTION_INDEX_OUT_OF_RANGE\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NOT_REGISTERED:
					printf("RPC_ERROR_FUNCTION_NOT_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED:
					printf("RPC_ERROR_FUNCTION_NO_LONGER_REGISTERED\n");
					break;
				case RakNet::RPC_ERROR_CALLING_CPP_AS_C:
					printf("RPC_ERROR_CALLING_CPP_AS_C\n");
					break;
				case RakNet::RPC_ERROR_CALLING_C_AS_CPP:
					printf("RPC_ERROR_CALLING_C_AS_CPP\n");
					break;
				}
				printf("Function: %s", p->data+2);
			}
			break;
		default:
			// The server knows the static data of all clients, so we can prefix the message
			// With the name data
			printf( "%s\n", p->data );

			// Relay the message.  We prefix the name for other clients.  This demonstrates
			// That messages can be changed on the server before being broadcast
			// Sending is the same as before
			sprintf_s( message, 2048, "%s", p->data );
			g_server->Send( message, (const int) strlen(message)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, p->systemAddress, true );

			break;
		}

		// We're done with the packet
		g_server->DeallocatePacket( p );
	}

	g_server->Shutdown( 300 );
	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface( g_server );

	delete masterUser;
	RakNet::RakString::FreeMemory();
	return 0;
}


// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}
