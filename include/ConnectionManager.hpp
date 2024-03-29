#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Configuration.hpp"

#define BUFFER_SIZE 4000

class ConnectionManager
{
	public:
		ConnectionManager();
		ConnectionManager(Configuration &config);
		ConnectionManager(std::vector<Server> servers);
		~ConnectionManager();
	
		void setUpServers();
		void runServers();
		void printServers();
	
	private:
		std::vector<Client> _clients;
		std::vector<Server> _servers;
		int _max_socket;
		int _count;
		fd_set _read_sockets, _write_sockets;
	
		void initSets();
		bool isServerSocket(int socket) const;
		void addServer(Server &server);
		void buildServers(std::vector<ServerConfig > &server_configs);
		void addClient(Client &client);
		void removeClient(int socket);
		Server *getServerBySocket(int socket);
		Client *getClientBySocket(int socket);
		void checkTimeouts();
};

#endif