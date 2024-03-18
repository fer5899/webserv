#include"../include/ServerConfig.hpp"

ServerConfig::ServerConfig()
{
}

ServerConfig::ServerConfig(const ServerConfig &other)
{
	*this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
	if (this != &other)
	{
		this->_config = other._config;
		this->_error_page = other._error_page;
		this->_locations = other._locations;
	}
	return *this;
}

ServerConfig::~ServerConfig()
{
}

int countWords(std::string line);

void ServerConfig::parseServerConfig(std::string line)
{
	// std::cout << line << std::endl;
	if (line.find(";") != std::string::npos)
		line = line.substr(0, line.find(";"));
	else if (line.find("}") == std::string::npos && line.find("location") == std::string::npos)
	{
		std::cerr << RED "Error: Invalid line - semicolon: " << line << RESET << std::endl;
		exit(1);
	}

	std::istringstream iss(line);
	if (line.find("error_page") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "error_page")
				continue;
			_error_page.push_back(word);
		}
	}
	else if (line.find("}") == std::string::npos && countWords(line) == 2)
	{
		std::string key;
		std::string value;

		iss >> key >> value;
		// std::cout << "key: " << key << " value: " << value << std::endl;
		_config[key] = value;
	}
}

void ServerConfig::addLocation(LocationConfig location)
{
	_locations.push_back(location);
}

std::vector<LocationConfig> &ServerConfig::getLocations()
{
	return _locations;
}

void ServerConfig::printServerConfig()
{
	for (std::map<std::string, std::string>::iterator it = _config.begin(); it != _config.end(); it++)
	{
		std::cout <<"-- " <<  it->first << " - " << it->second << std::endl;
	}
	std::cout << "--> Error pages:   " << _error_page.size() << std::endl;
	for (long unsigned int i = 0; i < _error_page.size(); i++)
	{
		std::cout << "  -- " << _error_page[i] << std::endl;
	}
	for (std::vector<LocationConfig>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		it->printLocationConfig();
	}
}

int ServerConfig::getPort() const
{
	try
	{
		// Data at listen has form host:port
		std::string listen = _config.at("listen");
		size_t pos = listen.find(":");
		in_addr_t host = 0;
		if (pos == std::string::npos
			&& inet_pton(AF_INET, listen.c_str(), &(host)) > 0)
			return 80;
		if (pos == listen.size() - 1)
		{
			std::cerr << RED "Error: Invalid port number - Server: " << getServerName() << RESET << std::endl;
			exit(1);
		}
		int port_num = 0;
		if (pos == std::string::npos)
			port_num = std::stoi(listen);
		else
			port_num = std::stoi(listen.substr(pos + 1));
		if (port_num < 0 || port_num > 65535)
		{
			std::cerr << RED "Error: Invalid port number - Server: " << getServerName() << RESET << std::endl;
			exit(1);
		}
		return port_num;
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "Error: No port specified or wrong value - Server: " << getServerName() << RESET << std::endl;
		exit(1);
	}
}

in_addr_t	ServerConfig::getHost() const
{
	try
	{
		// Data at listen has form host:port
		std::string listen = _config.at("listen");
		size_t pos = listen.find(":");
		if (pos == listen.size() - 1)
		{
			std::cerr << RED "Error: Invalid port number - Server: " << getServerName() << RESET << std::endl;
			exit(1);
		}
		in_addr_t host = 0;
		if (pos == std::string::npos)
		{
			if (inet_pton(AF_INET, listen.c_str(), &(host)) <= 0)
			{
				std::cerr << RED "Error: Invalid host - Server: " << getServerName() << RESET << std::endl;
				exit(1);
			}
		}
		else
		{
			if (inet_pton(AF_INET, listen.substr(0, pos).c_str(), &(host)) <= 0)
			{
				std::cerr << RED "Error: Invalid host - Server: " << getServerName() << RESET << std::endl;
				exit(1);
			}
		}
		return host;
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "Error: No host specified or wrong value - Server: " << getServerName() << RESET << std::endl;
		exit(1);
	}
}

std::string ServerConfig::getServerName() const
{
	try
	{
		return _config.at("server_name");
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "Error: No server_name specified" << RESET << std::endl;
		exit(1);
	}
}

std::string ServerConfig::getRoot() const
{
	try
	{
		std::string root = _config.at("root");
		if (!isAbsPath(root))
		{
			std::cerr << RED "Error: Invalid root path - Server: " << getServerName() << RESET << std::endl;
			exit(1);
		}
		return root;
	}
	catch(const std::exception& e)
	{
		std::cerr << RED "Error: No root specified - Server: " << getServerName() << RESET << std::endl;
		exit(1);
	}
}


std::string ServerConfig::getIndex() const
{
	try
	{
		std::string index = _config.at("index");
		if (index.find("/") != std::string::npos)
		{
			std::cerr << RED "Error: Invalid index - Server: " << getServerName() << RESET << std::endl;
			exit(1);
		}
		return index;
	}
	catch(const std::exception& e)
	{
		return "index.html";
	}
}

int	ServerConfig::getMaxBodySize() const
{
	try
	{
		return std::stoi(_config.at("client_max_body_size"));
	}
	catch(const std::out_of_range& e)
	{
		return 1000000;
	}
	catch(const std::invalid_argument& e)
	{
		std::cerr << RED "Error: Invalid max body size - Server: " << getServerName() << RESET << std::endl;
		exit(1);
	}
}

std::map<int, std::string> ServerConfig::getErrorPage() const
{
	std::map<int, std::string> error_page;
	size_t last_page = 0;
	for (size_t i = 0; i < _error_page.size(); i++)
	{
		// Iterate until an element ending in .html is found
		if (_error_page[i].find(".html") != std::string::npos)
		{
			// Check if its abspath
			if (!isAbsPath(_error_page[i]))
			{
				std::cerr << RED "Error: Invalid error page path - Server: " << getServerName() << RESET << std::endl;
				exit(1);
			}
			std::string error_page_path = _error_page[i];
			// Iterate from last_page to i and add all error codes
			for (size_t j = last_page; j < i; j++)
			{
				int error_code = 0;
				try
				{
					error_code = std::stoi(_error_page[j]);
				}
				catch(const std::exception& e)
				{
					std::cerr << RED "Error: Invalid error code - Server: " << getServerName() << RESET << std::endl;
					exit(1);
				}
				error_page[error_code] = error_page_path;
			}
			last_page = i + 1;
		}

	}
	return error_page;
}


