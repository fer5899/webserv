#include "../include/ConnectionManager.hpp"
#include "../include/Server.hpp"
#include "../include/Client.hpp"

ConnectionManager::ConnectionManager()
{
	this->_servers = std::vector<Server>();
	this->_max_socket = 0;
	this->_count = 0;
}

ConnectionManager::ConnectionManager(Configuration &config)
{
	this->_max_socket = 0;
	this->_count = 0;
	this->_servers = std::vector<Server>();
	buildServers(config.getServers());
	// Remove servers with repeated ports
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		for (std::vector<Server>::iterator it2 = it + 1; it2 != _servers.end(); it2++)
		{
			if (it->getPort() == it2->getPort())
			{
				it2 = _servers.erase(it2);
				it2 = it;
			}
		}
	}
}

ConnectionManager::ConnectionManager(std::vector<Server> servers) : _servers(servers) 
{
	this->_max_socket = 0;
	this->_count = 0;
}

ConnectionManager::~ConnectionManager() {}

void ConnectionManager::setUpServers()
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->setUpServer();
	}
}

void ConnectionManager::buildServers(std::vector<ServerConfig > &server_configs)
{
	for (size_t i = 0; i < server_configs.size(); i++)
	{
		Server server = Server(server_configs[i]);
		this->addServer(server);
	}
}

void ConnectionManager::runServers()
{
	fd_set read_sockets_copy, write_sockets_copy;

	this->initSets();
	struct timeval timer;
	
	while (1)
	{
		timer.tv_sec = 1;
		read_sockets_copy = this->_read_sockets;
		write_sockets_copy = this->_write_sockets;

		int	activity = select(this->_max_socket + 1, &read_sockets_copy, &write_sockets_copy, NULL, &timer);
		if (activity < 0)
		{
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= this->_max_socket; i++)
		{
			// New connections
			if (FD_ISSET(i, &read_sockets_copy) && this->isServerSocket(i))
			{
				int new_socket;

				if ((new_socket = accept(i, (struct sockaddr *)NULL, NULL)) < 0)
				{
					std::cerr << "Accept failed: " << strerror(errno) << std::endl;
					exit(EXIT_FAILURE);
				}
				FD_SET(new_socket, &this->_read_sockets);
				if (new_socket > this->_max_socket)
					this->_max_socket = new_socket;
				Client client = Client(this->getServerBySocket(i), new_socket);
				this->addClient(client);
			}

			// Read from client sockets
			else if (FD_ISSET(i, &read_sockets_copy))
			{
				Client *client = this->getClientBySocket(i);
				if (client == NULL)
				{
					std::cerr << "Client not found" << std::endl;
					exit(EXIT_FAILURE);
				}
		
				if (client->getRequest() == NULL)
				{
					client->setRequest(new Request(client->getServer()->getMaxBodySize()));
				}
				
				char buffer[BUFFER_SIZE];
				ssize_t bytesRead = recv(i, buffer, BUFFER_SIZE - 1, 0);
				if (bytesRead == -1)
				{
					std::cerr << "recv error: " << strerror(errno) << std::endl;
					close(i);
					FD_CLR(i, &this->_read_sockets);
					client->clearRequest();
					this->removeClient(i);
				}
				else if (bytesRead == 0)
				{
					close(i);
					FD_CLR(i, &this->_read_sockets);
					client->clearRequest();
					this->removeClient(i);
				}
				else
				{
					client->setLastReqTime();
					buffer[bytesRead] = '\0';
					std::string buffer_str(buffer);
					
					// Now we pass the buffer to the request handler and check if the request is complete
					if (client->getRequest()->parseRequest(buffer_str))
					{
						std::cout << std::endl << MAGENTA "-----HTTP Request: " RESET<< std::endl;
						std::cout << *(client->getRequest()) << std::endl;

						FD_CLR(i, &this->_read_sockets);
						FD_SET(i, &this->_write_sockets);
					}
				}
			}
			// Write to client sockets
			else if (FD_ISSET(i, &write_sockets_copy))
			{
				Client *client = this->getClientBySocket(i);
				if (client == NULL)
				{
					std::cerr << "Client not found" << std::endl;
					client->clearRequest();
					exit(EXIT_FAILURE);
				}
				client->setResponse(new Response(client, client->getRequest()));

				std::cout << MAGENTA "-----HTTP Response: " RESET<< std::endl;
				std::cout << *client->getResponse() << std::endl;

				ssize_t bytesSent = send(i, client->getResponse()->getHttpResponse().c_str(), client->getResponse()->getHttpResponse().length(), 0);
				if (bytesSent == -1)
				{
					std::cerr << "send error: " << strerror(errno) << std::endl;
					close(i);
					FD_CLR(i, &this->_write_sockets);
					client->clearRequest();
					client->clearResponse();
					this->removeClient(i);
				}
				else if (bytesSent == 0)
				{
					close(i);
					FD_CLR(i, &this->_write_sockets);
					client->clearRequest();
					client->clearResponse();
					this->removeClient(i);
				}
				else 
				{
					client->setLastReqTime();
					if (client->getRequest()->keepAlive() && client->getResponse()->keepAlive())
					{
						FD_CLR(i, &this->_write_sockets);
						FD_SET(i, &this->_read_sockets);
						
					}
					else
					{
						FD_CLR(i, &this->_write_sockets);
						close(i);
						this->removeClient(i);
					}
					client->clearRequest();
					client->clearResponse();
						
				}
			}
		}
		this->checkTimeouts();
	}
}

void ConnectionManager::initSets()
{
	FD_ZERO(&this->_read_sockets);
	FD_ZERO(&this->_write_sockets);

	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getSocket() > this->_max_socket)
			this->_max_socket = it->getSocket();
		FD_SET(it->getSocket(), &this->_read_sockets);
	}
}

bool ConnectionManager::isServerSocket(int socket) const
{
	for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		if (it->getSocket() == socket)
			return true;
	}
	return false;
}

void ConnectionManager::addServer(Server &server)
{
	this->_servers.push_back(server);
}

void ConnectionManager::addClient(Client &client)
{
	this->_clients.push_back(client);
}

void ConnectionManager::removeClient(int socket)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getSocket() == socket)
		{
			_clients.erase(it);
			break;
		}
	}
}

Server *ConnectionManager::getServerBySocket(int socket)
{
	for (unsigned long i = 0; i < this->_servers.size(); i++)
	{
		if (this->_servers[i].getSocket() == socket)
		{
			Server *s = &(this->_servers[i]);
			return s;
		}
	}
	return NULL;
}

Client *ConnectionManager::getClientBySocket(int socket)
{
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getSocket() == socket)
		{
			Client *c = &(this->_clients[i]);
			return c;
		}
	}
	return NULL;
}

void ConnectionManager::printServers()
{
	for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		it->printServer();
	}
}

void ConnectionManager::checkTimeouts()
{
	for(unsigned long i = 0; i < this->_clients.size(); i++)
	{
		if (time(NULL) - this->_clients[i].getLastReqTime() > CONN_TIMEOUT)
		{
			FD_CLR(this->_clients[i].getSocket(), &this->_read_sockets);
			FD_CLR(this->_clients[i].getSocket(), &this->_write_sockets);
			close(this->_clients[i].getSocket());
			this->removeClient(this->_clients[i].getSocket());
		}
	}
}


