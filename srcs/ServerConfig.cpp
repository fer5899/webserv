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
