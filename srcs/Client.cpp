#include "../include/Client.hpp"

Client::Client(Server server, int socket) : _server(server), _socket(socket) {}

Client::~Client() {}

int Client::getSocket() const
{
	return this->_socket;
}