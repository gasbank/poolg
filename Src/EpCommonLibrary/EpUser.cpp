#include "EpCommonLibraryPCH.h"
#include "EpUser.h"
#include "UnitBase.h"

NetworkIDManager& GetNetworkIdManager();
RakPeerInterface* GetRakPeer();


DataStructures::List<EpUser*> EpUser::users;
EpUser *EpUser::myUser;


// EpUser starts out with no unit. Also, track this pointer.
EpUser::EpUser()
: m_unit(0)
{
	users.Insert(this);
	m_systemAddress=UNASSIGNED_SYSTEM_ADDRESS;
}

// Remove myself from the static users list
EpUser::~EpUser()
{
	users.RemoveAtIndex(users.GetIndexOf(this));

	// When the user logs off his unit should go away too
	if (m_unit)
	{
		// Unfortunately BroadcastDestruction() cannot be called automatically in the destructor of Replica2, because virtual functions can not call to derived classes.
		// It is in the derived class QueryIsDestructionAuthority() that we give the client authority to network delete the object
		
		m_unit->BroadcastDestruction();

		delete m_unit;
	}

	// Store myEpUser so the client knows which user refers to him
	if (EpUser::myUser==this)
	{
		// Shouldn't ever see this
		printf("My user deleted.\n");
		UnitBase::myUnit=0;
	}
	else
	{
		printf("EpUser with address %s deleted.\n", m_systemAddress.ToString());
	}
}

// Helper function to free memory when someone disconnections.
void EpUser::DeleteUserByAddress(SystemAddress systemAddress)
{
	for (unsigned i=0; i < users.Size(); i++)
	{
		if (users[i]->getSystemAddress() == systemAddress)
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
		if (users[i]->getSystemAddress() == systemAddress)
		{
			return users[i];
		}
	}
	return 0;
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
	if (m_unit)
		bitStream->Write(m_unit->GetNetworkID());
	else
		bitStream->Write(UNASSIGNED_NETWORK_ID);
	bitStream->Write(m_systemAddress);
	return true;
}
// Implemented member of Replica2: Read what I wrote in Serialize() immediately above
void EpUser::Deserialize(RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp)
{
	NetworkID unitNetworkId;
	bitStream->Read(unitNetworkId);
	m_unit=(UnitBase*) GetNetworkIdManager().GET_OBJECT_FROM_ID(unitNetworkId);
	bitStream->Read(m_systemAddress);
	// The client stores a pointer to its own user, for convenience
	if (m_systemAddress==GetRakPeer()->GetExternalID(UNASSIGNED_SYSTEM_ADDRESS))
		EpUser::myUser=this;
	// The unit pointer may be NULL, since you spawn in units, rather than have them created immediately on connection as are EpUsers
	if (m_unit)
		printf( "EpUser at address %s updated. Has unit with name: %s.\n", m_systemAddress.ToString(), m_unit->getRepName().C_String() );
	else
		printf( "EpUser at address %s updated. No unit spawned.\n", m_systemAddress.ToString() );
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