#include "EpCommonLibraryPCH.h"
#include "UnitBase.h"

//DataStructures::List<UnitBase*> UnitBase::soldiers;
UnitBase *UnitBase::mySoldier;


UnitBase::UnitBase(void)
{
}

UnitBase::~UnitBase(void)
{
}

bool UnitBase::SerializeConstruction( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext )
{
	return true;
}

bool UnitBase::Serialize( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext )
{
	return true;
}

void UnitBase::Deserialize( RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp )
{

}