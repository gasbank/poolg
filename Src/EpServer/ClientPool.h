#pragma once

class ClientPool
{
public:
	ClientPool(void);
	~ClientPool(void);

	void addClient( SystemAddress sa ) { m_clients.push_back( sa ); }
	void removeClient( SystemAddress sa ) { m_clients.remove( sa ); }
	size_t getClientCount() const { return m_clients.size(); }
private:
	typedef std::list<SystemAddress> SystemAddressList;

	SystemAddressList m_clients;
};
