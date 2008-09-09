#pragma once

class UnitBase : public RakNet::Replica2
{
public:
	UnitBase(void);
	virtual ~UnitBase(void);

	// Implemented member of Replica2: This function encodes the identifier for this class, so the class factory can create it
	virtual bool SerializeConstruction( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext );
	// Implemented member of Replica2: Write the data members of this class. ReplicaManager2 works with pointers as well as any other kind of data
	virtual bool Serialize( RakNet::BitStream *bitStream, RakNet::SerializationContext *serializationContext );
	// Implemented member of Replica2: Read what I wrote in Serialize() immediately above
	virtual void Deserialize( RakNet::BitStream *bitStream, RakNet::SerializationType serializationType, SystemAddress sender, RakNetTime timestamp );


	const char* getRepName() const { return m_repName; }


	static UnitBase* mySoldier;

private:
	char m_repName[128];
	float m_syncX;
	float m_syncY;
	float m_syncZ;
};
