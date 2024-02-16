#include "../include/Server.hpp"

Server::Server(int port) : _port(port) {}

Server::~Server() {}

void Server::setUpServer()
{
	if ((this->_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port selected
	int opt = 1;
	setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		
	int flags = fcntl(this->_server_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);	// Add non-blocking flag and set close-on-exec flag, how the subject says
	if (flags == -1)
	{
		std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	// flags = fcntl(this->_server_socket, F_SETFD, FD_CLOEXEC);	// Set close-on-exec flag, how it should be
	// if (flags == -1)
	// {
	// 	std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_address.sin_port = htons(this->_port);

	if (bind(this->_server_socket, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(this->_server_socket, 512) < 0)
	{
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

int Server::getSocket() const
{
	return _server_socket;
} 