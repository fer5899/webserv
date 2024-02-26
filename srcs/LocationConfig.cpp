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

void LocationConfig::parseLocationConfig(std::string line)
{
	//std::cout << line << std::endl;

	std::istringstream iss(line);
	if(line.find("methods") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "methods")
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
	else
	{
		std::string key;
		std::string value;

		iss >> key >> value;
		_locConfig[key] = value;
	}
}

void LocationConfig::printLocationConfig()
{
	for (std::map<std::string, std::string>::iterator it = _locConfig.begin(); it != _locConfig.end(); it++)
	{
		std::cout << "----" << it->first << " " << it->second << std::endl;
	}
	std::cout << "Methods: " << std::endl;
	for (std::vector<std::string>::iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		std::cout <<"----" <<  *it << std::endl;
	}
	std::cout << "CgiPath: " << std::endl;
	for (std::vector<std::string>::iterator it = _cgiPath.begin(); it != _cgiPath.end(); it++)
	{
		std::cout <<"----" <<  *it << std::endl;
	}
	std::cout << "CgiExt: " << std::endl;
	for (std::vector<std::string>::iterator it = _cgiExt.begin(); it != _cgiExt.end(); it++)
	{
		std::cout <<"----" <<  *it << std::endl;
	}
}