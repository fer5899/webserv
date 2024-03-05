#include"../include/LocationConfig.hpp"

LocationConfig::LocationConfig()
{
}


LocationConfig::LocationConfig(const LocationConfig &other)
{
	*this = other;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
	if (this != &other)
	{
		this->_path = other._path;
		this->_locConfig = other._locConfig;
		this->_methods = other._methods;
		this->_cgiPath = other._cgiPath;
		this->_cgiExt = other._cgiExt;
	}
	return *this;
}

LocationConfig::~LocationConfig()
{
}

int countWords(std::string line);

void LocationConfig::parseLocationConfig(std::string line)
{
	//std::cout << line << std::endl;
	if (line.find(";") != std::string::npos)
		line = line.substr(0, line.find(";"));
	else if (line.find("}") == std::string::npos)
	{
		std::cerr << RED "Error: Invalid line - semicolon: " << line << RESET << std::endl;
		exit(1);
	}

	std::istringstream iss(line);
	if(line.find("allow_methods") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "allow_methods")
				continue;
			_methods.push_back(word);
		}
	}
	else if(line.find("cgi_path") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "cgi_path")
				continue;
			_cgiPath.push_back(word);
		}
	}
	else if(line.find("cgi_ext") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "cgi_ext")
				continue;
			_cgiExt.push_back(word);
		}
	}
	else if(line.find("}") == std::string::npos && countWords(line) == 2)
	{
		std::string key;
		std::string value;

		iss >> key >> value;
		_locConfig[key] = value;
	}
}

void LocationConfig::setPath(std::string line)
{
	std::istringstream iss(line);
	std::string location;
	std::string word;
	
	iss >> location >> word;
	if (!word.empty())
		_path = word;
}

void LocationConfig::printLocationConfig()
{
	std::cout << "-->Location: " << _path << std::endl;
	for (std::map<std::string, std::string>::iterator it = _locConfig.begin(); it != _locConfig.end(); it++)
	{
		std::cout << "  -- " << it->first << " - " << it->second << std::endl;
	}
	std::cout << "  -->Methods: " << std::endl;
	for (std::vector<std::string>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		std::cout <<"    -- " <<  *it << std::endl;
	}
	std::cout << "  -->CgiPath: " << std::endl;
	for (std::vector<std::string>::iterator it = _cgiPath.begin(); it != _cgiPath.end(); it++)
	{
		std::cout <<"    -- " <<  *it << std::endl;
	}
	std::cout << "  -->CgiExt: " << std::endl;
	for (std::vector<std::string>::iterator it = _cgiExt.begin(); it != _cgiExt.end(); it++)
	{
		std::cout <<"    --" <<  *it << std::endl;
	}
}