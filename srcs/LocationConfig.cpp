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
		this->_return = other._return;
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
	else if(line.find("return") != std::string::npos)
	{
		std::string word;
		while(iss >> word)
		{
			if (word == "return")
				continue;
			_return.push_back(word);
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
	std::cout << "  -->Return: " << std::endl;
	for (std::vector<std::string>::iterator it = _return.begin(); it != _return.end(); it++)
	{
		std::cout <<"    --" <<  *it << std::endl;
	}
}

std::string LocationConfig::getPath() const
{
	if (!isAbsPath(_path))
	{
		std::cerr << RED "Error: Invalid location path: " << _path << RESET << std::endl;
		exit(1);
	}
	return _path;
}

std::string LocationConfig::getRoot() const
{
	try
	{
		std::string root = _locConfig.at("root");
		if (!isAbsPath(root))
		{
			std::cerr << RED "Error: Invalid root path - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return root;
	}
	catch(const std::exception& e)
	{
		return "";
	}
}

std::string LocationConfig::getIndex() const
{
	try
	{
		std::string index = _locConfig.at("index");
		if (index.find("/") != std::string::npos)
		{
			std::cerr << RED "Error: Invalid index path - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return index;
	}
	catch(const std::exception& e)
	{
		return "";
	}
}

std::string LocationConfig::getAlias() const
{
	try
	{
		std::string alias = _locConfig.at("alias");
		if (!isAbsPath(alias))
		{
			std::cerr << RED "Error: Invalid alias path - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return alias;
	}
	catch(const std::exception& e)
	{
		return "";
	}
}

std::vector<std::string> LocationConfig::getMethods() const
{
	// Check if the methods are valid
	for (std::vector<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); it++)
	{
		if (*it != "GET" && *it != "POST" && *it != "DELETE")
		{
			std::cerr << RED "Error: Invalid method: " << *it << " - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
	}
	return _methods;
}

bool LocationConfig::getAutoindex() const
{
	try
	{
		std::string autoindex = _locConfig.at("autoindex");
		if (autoindex == "true")
			return true;
		else if (autoindex == "false")
			return false;
		else
		{
			std::cerr << RED "Error: Invalid autoindex value - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
	}
	catch(const std::exception& e)
	{
		return false;
	}
}

int LocationConfig::getRedirCode() const
{
	try
	{
		if (_return.empty())
			return 0;
		if (_return.size() != 2)
		{
			std::cerr << RED "Error: Invalid return value - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		std::string redir_code = _return[0];
		if (redir_code != "301" 
			&& redir_code != "302" 
			&& redir_code != "303" 
			&& redir_code != "307" 
			&& redir_code != "308")
		{
			std::cerr << RED "Error: Invalid redir_code value - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return std::stoi(redir_code);
	}
	catch(const std::exception& e)
	{
		return 0;
	}
}

std::string LocationConfig::getRedirUrl() const
{
	try
	{
		if (_return.empty())
			return "";
		if (_return.size() != 2)
		{
			std::cerr << RED "Error: Invalid return value - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		std::string redir_url = _return[1];
		// Check that the redir_url is a url
		if (redir_url.find("http://") == std::string::npos && redir_url.find("https://") == std::string::npos)
		{
			std::cerr << RED "Error: Invalid redir_url path - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return redir_url;
	}
	catch(const std::exception& e)
	{
		return "";
	}
}

std::string LocationConfig::getUploadStore() const
{
	try
	{
		std::string upload_store = _locConfig.at("upload_store");
		if (!isAbsPath(upload_store))
		{
			std::cerr << RED "Error: Invalid upload_store path - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
		return upload_store;
	}
	catch(const std::exception& e)
	{
		return "";
	}
}

std::map<std::string, std::string> LocationConfig::getCgi() const
{
	std::map<std::string, std::string> cgi;
	if (_cgiPath.empty() || _cgiExt.empty())
		return cgi;
	if (_cgiPath.size() != _cgiExt.size())
	{
		std::cerr << RED "Error: Invalid cgi_path and cgi_ext - Location: " << getPath() << RESET << std::endl;
		exit(1);
	}
	// Check if the cgi_path is valid
	for (std::vector<std::string>::const_iterator it = _cgiPath.begin(); it != _cgiPath.end(); it++)
	{
		if (access(it->c_str(), X_OK) == -1)
		{
			std::cerr << RED "Error: Invalid cgi_path: " << *it << " - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
	}
	// Check if the cgi_ext is valid
	for (std::vector<std::string>::const_iterator it = _cgiExt.begin(); it != _cgiExt.end(); it++)
	{
		if ((*it)[0] != '.' || (*it).substr(1).find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos)
		{
			std::cerr << RED "Error: Invalid cgi_ext: " << *it << " - Location: " << getPath() << RESET << std::endl;
			exit(1);
		}
	}
	for (size_t i = 0; i < _cgiPath.size(); i++)
	{
		cgi[_cgiExt[i]] = _cgiPath[i];
	}
	return cgi;
}

