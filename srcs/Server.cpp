#include "../include/Server.hpp"

Server::Server(std::string server_name, std::string host, int port, std::map<int, std::string> error_page, int max_body_size, std::vector<Location> &locations, int socket, std::string root, std::string index) : _locations(locations)
{
	_server_name = server_name;
	_host = host;
	_port = port;
	_error_page = error_page;
	_max_body_size = max_body_size;
	_socket = socket;
	_root = root;
	_index = index;
}

Server::~Server()
{
}

std::string	Server::getServerName()
{
	return _server_name;
}

std::string	Server::getHost()
{
	return _host;
}

int	Server::getPort()
{
	return _port;
}

std::map<int, std::string>	Server::getDefError()
{
	return _error_page;
}

int	Server::getMaxBodySize()
{
	return _max_body_size;
}

std::vector<Location>	Server::getLocations()
{
	return _locations;
}

int	Server::getSocket()
{
	return _socket;
}

std::string	Server::getRoot()
{
	return _root;
}

std::string	Server::getIndex()
{
	return _index;
}


