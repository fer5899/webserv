#include "../include/Client.hpp"


Client::Client(std::vector<Server> *server_vector, int socket)
{
	_socket = socket;
	_server_vector = server_vector;
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
	_server_vector = other._server_vector;
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

Server		*Client::getServer(std::string server_name)
{
	for (std::vector<Server>::iterator it = _server_vector->begin(); it != _server_vector->end(); it++)
	{
		if (it->getServerName() == server_name)
			return &(*it);
	}
	return &(*(_server_vector->begin()));
}

std::vector<Server>	*Client::getServerVector()
{
	return _server_vector;
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

void		Client::clearRequest()
{
	if (_request)
		delete _request;
	_request = NULL;
}

void		Client::clearResponse()
{
	if (_response)
		delete _response;
	_response = NULL;
}

