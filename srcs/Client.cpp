#include "../include/Client.hpp"

Client::Client(Server server, int socket) : _server(server), _socket(socket), _requestHandler(NULL) {}

Client::~Client() {}

int Client::getSocket() const
{
	return this->_socket;
}

RequestHandler *Client::getRequestHandler() const
{
	return this->_requestHandler;
}

void Client::setRequestHandler(void)
{
	this->_requestHandler = new RequestHandler();
}
