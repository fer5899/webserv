#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include "Server.hpp"
#include "Client.hpp"

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
		std::vector<Client> _clients;
		int _max_socket;
		int _count;
		fd_set _read_sockets, _write_sockets;
	
		void initSets();
		bool isServerSocket(int socket) const;
		void addClient(Client client);
		void removeClient(int socket);
		Server getServerBySocket(int socket) const;
		Client getClientBySocket(int socket) const;
};

#endif