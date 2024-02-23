#include "../include/Server.hpp"

Server::Server()
{
	_server_name = "";
	_host = "";
	_port = 0;
	_error_page = std::map<int, std::string>();
	_max_body_size = 0;
	_locations = std::vector<Location>();
	_socket = 0;
	_root = "";
	_index = "";
}

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

Location	&Server::getLocation(int i)
{
	if (i < 0 || i >= (int)_locations.size())
		throw std::out_of_range("Location index out of range");
	return _locations[i];
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

void	Server::setServerName(std::string server_name)
{
	_server_name = server_name;
}

void	Server::setHost(std::string host)
{
	_host = host;
}

void	Server::setPort(int port)
{
	_port = port;
}

void	Server::setDefError(std::map<int, std::string> error_page)
{
	_error_page = error_page;
}

void	Server::setMaxBodySize(int max_body_size)
{
	_max_body_size = max_body_size;
}

void	Server::setLocations(std::vector<Location> &locations)
{
	_locations = locations;
}

void	Server::setSocket(int socket)
{
	_socket = socket;
}

void	Server::setRoot(std::string root)
{
	_root = root;
}

void	Server::setIndex(std::string index)
{
	_index = index;
}

void	Server::addLocation(Location &location)
{
	_locations.push_back(location);
}

void	Server::setErrorPage(int error_code, std::string error_page_path)
{
	_error_page[error_code] = error_page_path;
}


