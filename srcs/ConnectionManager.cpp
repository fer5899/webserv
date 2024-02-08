#include "../include/ConnectionManager.hpp"
#include "../include/Server.hpp"

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

void ConnectionManager::runServers()
{
	fd_set read_sockets_copy, write_sockets_copy;

	this->initSets();
	std::cout << "Running servers: waiting for connections..." << std::endl;
	
	while (1)
	{
		std::cout << "Count: " << this->_count++ << "\n";
		read_sockets_copy = this->_read_sockets;
		write_sockets_copy = this->_write_sockets;

		int	activity = select(this->_max_socket + 1, &read_sockets_copy, &write_sockets_copy, NULL, NULL);
		if (activity < 0)
		{
			std::cerr << "Select error: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= this->_max_socket; i++)
		{
			//New connections
			if (FD_ISSET(i, &read_sockets_copy) && this->isServerSocket(i))
			{
				int new_socket;

				if ((new_socket = accept(i, (struct sockaddr *)NULL, NULL)) < 0)
				{
					std::cerr << "Accept failed: " << strerror(errno) << std::endl;
					exit(EXIT_FAILURE);
				}
				std::cout << "New connection, socket fd is " << new_socket << std::endl;
				FD_SET(new_socket, &this->_read_sockets);
				if (new_socket > this->_max_socket)
					this->_max_socket = new_socket;
			}

			// Read from client sockets
			else if (FD_ISSET(i, &read_sockets_copy))
			{
				std::string httpRequest;
				// Right now it is a loop for example porpuses
				/* I should read the request on chunks and send it to the 
					httpresponse parser, and when it tells me that the 
					request is complete, I should could the connectionm, 
					build the response and change the fd from read to write */
				while (true)
				{
					char buffer[BUFFER_SIZE];
					ssize_t bytesRead = recv(i, buffer, BUFFER_SIZE - 1, 0);
					if (bytesRead == -1)
					{
						std::cerr << "recv error: " << strerror(errno) << std::endl;
						close(i);
						FD_CLR(i, &this->_read_sockets);
						break;
					}
					else if (bytesRead == 0 && httpRequest.empty())
					{
						std::cout << "bytesRead == 0, closing connection" << std::endl;
						close(i);
						FD_CLR(i, &this->_read_sockets);
						break;
					}
					else if (bytesRead < BUFFER_SIZE - 1) 
					{
						buffer[bytesRead] = '\0';
						httpRequest += buffer;
						std::cout << "Received complete HTTP Request:\n" << httpRequest << std::endl;
						FD_CLR(i, &this->_read_sockets);
						FD_SET(i, &this->_write_sockets);
						break;
					}
					else 
					{
						buffer[bytesRead] = '\0';
						httpRequest += buffer;
					}
				}
			}
			//Write to client sockets
			else if (FD_ISSET(i, &write_sockets_copy))
			{
				//std::cout << "Write to client socket" << std::endl;
				const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
				ssize_t bytesSent = send(i, response, strlen(response), 0);
				if (bytesSent == -1)
				{
					std::cerr << "send error: " << strerror(errno) << std::endl;
					close(i);
					FD_CLR(i, &this->_write_sockets);
				}
				else if (bytesSent == 0)
				{
					close(i);
					FD_CLR(i, &this->_write_sockets);
				}
				else 
				{
					std::cout << "Data sent" << std::endl;
					close(i);
					FD_CLR(i, &this->_write_sockets);
					std::cout << "Connection closed" << std::endl;
				}
			}
		}
		
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

