#include "../include/Client.hpp"


Client::Client(Server *server, int socket) : _socket(socket), _server(server)
{
	_request = NULL;
	_response = NULL;
}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	_socket = other._socket;
	_server = other._server;
	_request = other._request;
	_response = other._response;
	return *this;
}

Client::~Client()
{
	// if (_request)
	// 	delete _request;
	// if (_response)
	// 	delete _response;
}

Request		*Client::getRequest()
{
	return _request;
}

Response	*Client::getResponse()
{
	return _response;
}

int			Client::getSocket() const
{
	return _socket;
}

Server		*Client::getServer()
{
	return _server;
}

void		Client::setRequest(Request *request)
{
	_request = request;
}

void		Client::setResponse(Response *response)
{
	_response = response;
}

