#include "EpCommonLibraryPCH.h"
#include "EpUser.h"
#include "UnitBase.h"

NetworkIDManager& GetNetworkIdManager();
RakPeerInterface* GetRakPeer();


DataStructures::List<EpUser*> EpUser::users;
EpUser *EpUser::myUser;


// EpUser starts out with no soldier. Also, track this pointer.
EpUser::EpUser()
{
	soldier=0;
	users.Insert(this);
	systemAddress=UNASSIGNED_SYSTEM_ADDRESS;
}

// Remove myself from the static users list
EpUser::~EpUser()
{
	users.RemoveAtIndex(users.GetIndexOf(this));

	// When the user logs off his soldier should go away too
	if (soldier)
	{
		// Unfortunately BroadcastDestruction() cannot be called automatically in the destructor of Replica2, because virtual functions can not call to derived classes.
		// It is in the derived class QueryIsDestructionAuthority() that we give the client authority to network delete the object
		
		soldier->BroadcastDestruction();

		delete soldier;
	}

	// Store myEpUser so the client knows which user refers to him
	if (EpUser::myUser==this)
	{
		// Shouldn't ever see this
		printf("My user deleted.\n");
		UnitBase::mySoldier=0;
	}
	else
	{
		printf("EpUser with address %s deleted.\n", systemAddress.ToString());
	}
}

// Helper function to free memory when someone disconnections.
void EpUser::DeleteUserByAddress(SystemAddress systemAddress)
{
	for (unsigned i=0; i < users.Size(); i++)
	{
		if (users[i]->systemAddress==systemAddress)
		{
			// Unfortunately BroadcastDestruction() cannot be called automatically in the destructor of Replica2, because virtual functions can not call to derived classes.
			// It is in the derived class QueryIsDestructionAuthority() that we give the client authority to network delete the object
			users[i]->BroadcastDestruction();

			delete users[i];
			return;
		}
	}
}

// Helper function to lookup the user in users by systemAddress
EpUser* EpUser::GetUserByAddress(SystemAddress systemAddress)
{
	for (unsigned i=0; i < users.Size(); i++)
	{
		if (users[i]->systemAddress==systemAddress)
		{
			return users[i];
		}
	}
	return 0;
}

// Each user can have a soldier. Create my soldier if I don't have one already
UnitBase* EpUser::CreateSoldier(void)
{
	if (soldier==0)
	{
		soldier = new UnitBase( UT_UNITBASE );
		return soldier;
	}
	return 0;
}
// Return my soldier
UnitBase* EpUser::GetMySoldier(void) const
{
	return soldier;
}

// Implemented member of Replica2: This function encodes the identifier for this class, so the class factory can create it
bool EpUser::SerializeConstruction(RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext)
{
	RakNet::StringTable::Instance()->EncodeString("EpUser",128,bitStream);
	return true;
}
// Implemented member of Replica2: Write the data members of this class. ReplicaManager2 works with pointers as well as any other kind of data
bool EpUser::Serialize(RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext)
{
	if (soldier)
		bitStream->Write(soldier->GetNetworkID());
	else
		bitStream->Write(UNASSIGNED_NETWORK_ID);
	bitStream->Write(systemAddress);
	return true;
}
// Implemented member of Replica2: Read what I wrote in Serialize() immediately above
void EpUser::Deserialize(RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp)
{
	NetworkID soldierNetworkId;
	bitStream->Read(soldierNetworkId);
	soldier=(UnitBase*) GetNetworkIdManager().GET_OBJECT_FROM_ID(soldierNetworkId);
	bitStream->Read(systemAddress);
	// The client stores a pointer to its own user, for convenience
	if (systemAddress==GetRakPeer()->GetExternalID(UNASSIGNED_SYSTEM_ADDRESS))
		EpUser::myUser=this;
	// The soldier pointer may be NULL, since you spawn in soldiers, rather than have them created immediately on connection as are EpUsers
	if (soldier)
		printf( "EpUser at address %s updated. Has soldier with name: %s.\n", systemAddress.ToString(), soldier->getRepName().C_String() );
	else
		printf( "EpUser at address %s updated. No soldier spawned.\n", systemAddress.ToString() );
}

void EpUser::DeleteAllUsers()
{

	while ( users.Size() )
	{
		// This function is called during the termination process of client-side,
		// so you do not need to broadcast destruction event. It will done by server-side.
		// (But, is it okay?)

		//users[0]->BroadcastDestruction();

		delete users[0];
	}
}