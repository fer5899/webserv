#include "../include/Client.hpp"


Client::Client(int socket, Server &server) : _socket(socket), _server(server)
{
}

Client::~Client()
{
}

Request		*Client::getRequest() const
{
	return _request;
}

Response	*Client::getResponse() const
{
	return _response;
}

int			Client::getSocket() const
{
	return _socket;
}

Server		&Client::getServer()
{
	return _server;
}

void		Client::setRequest(Request *request)
{
	_request = request;
}

