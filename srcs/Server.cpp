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

Server::Server(const Server &other)
{
	*this = other;
}

Server &Server::operator=(const Server &other)
{
	_server_name = other._server_name;
	_port = other._port;
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
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
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


// PARTE DE FERNANDO

// std::map<std::string, std::string> decodeUrlEncodedFormData(const std::string& encodedFormData)
// {
// 	std::map<std::string, std::string> decodedData;
// 	std::istringstream iss(encodedFormData);

// 	std::string pair;
// 	while (std::getline(iss, pair, '&'))
// 	{
// 		std::istringstream pairStream(pair);
// 		std::string key_value;
// 		while (std::getline(pairStream, key_value, '='))
// 		{
// 			std::string decodedKey = "";
// 			std::string decodedValue = "";
// 			 unsigned int decodeChar;
// 			 int len = key_value.length();
// 			 for (int i = 0; i < len; i++)
// 			{
// 				 if (key_value[i] == '%')
// 				{
// 					 sscanf(key_value.substr(i + 1, 2).c_str(), "%x", &decodeChar);
// 					 decodedKey += static_cast<char>(decodeChar);
// 					 i += 2;
// 				 }
// 				 else if (key_value[i] == '+')
// 				{
// 					 decodedKey += ' ';
// 				 }
// 				 else
// 				{
// 					 decodedKey += key_value[i];
// 				 }
// 			 }
// 			 std::getline(pairStream, key_value, '=');
// 			 len = key_value.length();
// 			 for (int i = 0; i < len; i++)
// 			{
// 				 if (key_value[i] == '%')
// 				{
// 					 sscanf(key_value.substr(i + 1, 2).c_str(), "%x", &decodeChar);
// 					 decodedValue += static_cast<char>(decodeChar);
// 					 i += 2;
// 				 }
// 				 else if (key_value[i] == '+')
// 				{
// 					 decodedValue += ' ';
// 				 }
// 				 else
// 				{
// 					 decodedValue += key_value[i];
// 				 }
// 			 }
// 			 decodedData[decodedKey] = decodedValue;
// 		 }
// 	 }

// 	 return decodedData;
// }

// int main()
// {
// 	 std::string encodedData = "q=hello+world&lang=en%20o%20es&foo=bar&foo=baz&foo=qux&name=John+Doe&age=25&email=john%40doe.com";
// 	 std::map<std::string, std::string> decodedData = decodeUrlEncodedFormData(encodedData);

// 	 // Imprimir los pares clave-valor decodificados
// 	for (std::map<std::string, std::string>::iterator it = decodedData.begin(); it != decodedData.end(); ++it)
// 	{
// 		td::cout << "Clave: " << it->first << ", Valor: " << it->second << std::endl;
// 	}

// 	 return 0;
// }