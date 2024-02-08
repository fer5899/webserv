#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include "Server.hpp"

#define BUFFER_SIZE 100

class ConnectionManager
{
public:
	ConnectionManager(std::vector<Server> servers);
	~ConnectionManager();

	void setUpServers();
	void runServers();

private:
	std::vector<Server> _servers;
	int _max_socket;
	int _count;
	fd_set _read_sockets, _write_sockets;

	void initSets();
	bool isServerSocket(int socket) const;

};

#endif