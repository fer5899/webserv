#include "../include/Client.hpp"

Client::Client(Server server, int socket) : _server(server), _socket(socket), _Request(NULL) {}

Client::~Client() {}

int Client::getSocket() const
{
	return this->_socket;
}

Request *Client::getRequest() const
{
	return this->_Request;
}

void Client::setRequest(void)
{
	this->_Request = new Request();
}