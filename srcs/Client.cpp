#include "../include/Client.hpp"


Client::Client(int socket, Server &server) : _socket(socket), _server(server), _req_count(0)
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

Server		&Client::getServer() const
{
	return _server;
}

