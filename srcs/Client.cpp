#include "../include/Client.hpp"


Client::Client(Server *server, int socket) : _socket(socket), _server(server)
{
	_request = NULL;
	_response = NULL;
	_last_req_time = time(NULL);
	// std::cout << "FIRST Last req time set to: " << _last_req_time << std::endl;
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
	_last_req_time = other._last_req_time;
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

const time_t	&Client::getLastReqTime() const
{
	return _last_req_time;
}

void		Client::setLastReqTime()
{
	_last_req_time = time(NULL);
	// std::cout << "Last req time set to: " << _last_req_time << std::endl;
}

