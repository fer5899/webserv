#include "../include/Server.hpp"

Server::Server()
{
	_server_name = "";
	_port = 0;
	_error_page = std::map<int, std::string>();
	_max_body_size = 0;
	_locations = std::vector<Location>();
	_server_socket = 0;
	_root = "";
	_index = "";
}

Server::Server(int port) : _port(port)
{
	_server_name = "";
	_error_page = std::map<int, std::string>();
	_max_body_size = 0;
	_locations = std::vector<Location>();
	_server_socket = 0;
	_root = "";
	_index = "";
}

Server::Server(ServerConfig &config)
{
	_server_name = config.getServerName();
	_port = config.getPort();
	_host = config.getHost();
	_error_page = config.getErrorPage();
	_max_body_size = config.getMaxBodySize();
	_server_socket = 0;
	_root = config.getRoot();
	_index = config.getIndex();
	_locations = std::vector<Location>();
	buildLocations(config.getLocations(), config);
}

Server::Server(const Server &other)
{
	*this = other;
}

Server &Server::operator=(const Server &other)
{
	_server_name = other._server_name;
	_port = other._port;
	_host = other._host;
	_error_page = other._error_page;
	_max_body_size = other._max_body_size;
	_locations = other._locations;
	_server_socket = other._server_socket;
	_root = other._root;
	_index = other._index;
	_address = other._address;
	return *this;
}

Server::~Server()
{
}

void	Server::buildLocations(std::vector<LocationConfig> location_configs, ServerConfig server_config)
{
	for (size_t i = 0; i < location_configs.size(); i++)
	{
		Location location = Location(location_configs[i], server_config);
		this->addLocation(location);
	}
}

void Server::setUpServer()
{
	if ((this->_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port selected
	int opt = 1;
	setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		
	int flags = fcntl(this->_server_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);	// Add non-blocking flag and set close-on-exec flag, how the subject says
	if (flags == -1)
	{
		std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	// flags = fcntl(this->_server_socket, F_SETFD, FD_CLOEXEC);	// Set close-on-exec flag, how it should be
	// if (flags == -1)
	// {
	// 	std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }
	this->_address.sin_family = AF_INET;
	if (inet_pton(AF_INET, this->getHost().c_str(), &this->_address.sin_addr.s_addr) <= 0)
	{
		std::cerr << "Wrong host in configuration file" << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_address.sin_port = htons(this->_port);


	if (bind(this->_server_socket, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(this->_server_socket, 512) < 0)
	{
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::string	Server::getServerName() const
{
	return _server_name;
}

int	Server::getPort() const
{
	return _port;
}

int	Server::getSocket() const
{
	return _server_socket;
} 

std::map<int, std::string>	Server::getDefError() const
{
	return _error_page;
}

size_t	Server::getMaxBodySize() const
{
	return _max_body_size;
}

std::vector<Location>	&Server::getLocations()       
{
	return _locations;
}

Location	&Server::getLocation(int i)
{
	if (i < 0 || i >= (int)_locations.size())
		throw std::out_of_range("Location index out of range");
	return _locations[i];
}

std::string	Server::getRoot() const
{
	return _root;
}

std::string	Server::getIndex() const
{
	return _index;
}

void	Server::setServerName(std::string server_name)
{
	_server_name = server_name;
}

void	Server::setPort(int port)
{
	_port = port;
}

void	Server::setDefError(std::map<int, std::string> error_page)
{
	_error_page = error_page;
}

void	Server::setMaxBodySize(size_t max_body_size)
{
	_max_body_size = max_body_size;
}

void	Server::setLocations(std::vector<Location> &locations)
{
	_locations = locations;
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

void	Server::printServer()
{
	std::cout << "--------------------------------" << std::endl;
	std::cout << "Server name: " << _server_name << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Error pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = _error_page.begin(); it != _error_page.end(); it++)
	{
		std::cout << "Error code: " << it->first << ", Error page: " << it->second << std::endl;
	}
	std::cout << "Max body size: " << _max_body_size << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Index: " << _index << std::endl;
	std::cout << "Locations: " << std::endl;
	printLocations();
	std::cout << "--------------------------------" << std::endl;
}

void	Server::printLocations()
{
	for (size_t i = 0; i < _locations.size(); i++)
	{
		_locations[i].printLocation();
	}
}

std::string	Server::getHost() const
{
	return _host;
}

