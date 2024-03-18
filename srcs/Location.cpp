#include "../include/Location.hpp"

Location::Location()
{
	_path = "";
	_root = "";
	_alias = "";
	_methods = std::vector<std::string>();
	_autoindex = false;
	_index = "";
	_redir_code = 0;
	_redir_url = "";
	_upload_store = "";
	_cgi = std::map<std::string, std::string>();
}

Location::Location(LocationConfig &config, ServerConfig &server_config)
{
	_path = config.getPath();

	if (config.getRoot().empty())
		_root = server_config.getRoot();
	else
		_root = config.getRoot();

	_alias = config.getAlias();
	_methods = config.getMethods();
	_autoindex = config.getAutoindex();

	if (config.getIndex().empty())
		_index = server_config.getIndex();
	else
		_index = config.getIndex();

	_redir_code = config.getRedirCode();
	_redir_url = config.getRedirUrl();
	_upload_store = config.getUploadStore();
	_cgi = config.getCgi();
}


Location::Location(const Location &other)
{
	_path = other._path;
	_root = other._root;
	_alias = other._alias;
	_methods = other._methods;
	_autoindex = other._autoindex;
	_index = other._index;
	_redir_code = other._redir_code;
	_redir_url = other._redir_url;
	_upload_store = other._upload_store;
	_cgi = other._cgi;
}

Location	&Location::operator=(const Location &other)
{
	if (this != &other)
	{
		_path = other._path;
		_root = other._root;
		_alias = other._alias;
		_methods = other._methods;
		_autoindex = other._autoindex;
		_index = other._index;
		_redir_code = other._redir_code;
		_redir_url = other._redir_url;
		_upload_store = other._upload_store;
		_cgi = other._cgi;
	}
	return *this;
}

Location::~Location()
{
}

std::string	Location::getPath()
{
	return _path;
}

std::string	Location::getRoot()
{
	return _root;
}

std::string	Location::getAlias()
{
	return _alias;
}

std::vector<std::string>	Location::getMethods()
{
	return _methods;
}

bool	Location::getAutoindex()
{
	return _autoindex;
}

std::string	Location::getIndex()
{
	return _index;
}

int	Location::getRedirCode()
{
	return _redir_code;
}

std::string	Location::getRedirUrl()
{
	return _redir_url;
}

std::string	Location::getUploadStore()
{
	return _upload_store;
}

std::map<std::string, std::string>	Location::getCgi()
{
	return _cgi;
}

void	Location::setPath(std::string path)
{
	_path = path;
}

void	Location::setRoot(std::string root)
{
	_root = root;
}

void	Location::setAlias(std::string alias)
{
	_alias = alias;
}

void	Location::setMethods(std::vector<std::string> methods)
{
	_methods = methods;
}

void	Location::addMethod(std::string method)
{
	_methods.push_back(method);
}

void	Location::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
}

void	Location::setIndex(std::string index)
{
	_index = index;
}

void	Location::setRedirCode(int redir_code)
{
	_redir_code = redir_code;
}

void	Location::setRedirUrl(std::string redir_url)
{
	_redir_url = redir_url;
}

void	Location::setUploadStore(std::string upload_store)
{
	_upload_store = upload_store;
}

void	Location::setCgi(std::map<std::string, std::string> cgi)
{
	_cgi = cgi;
}

void	Location::printLocation()
{
	std::cout << "----------------" << std::endl;
	std::cout << "Location path: " << _path << std::endl;
	std::cout << "Location root: " << _root << std::endl;
	std::cout << "Location alias: " << _alias << std::endl;
	std::cout << "Location methods: ";
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << _methods[i] << " ";
	std::cout << std::endl;
	std::cout << "Location autoindex: " << _autoindex << std::endl;
	std::cout << "Location index: " << _index << std::endl;
	std::cout << "Location redir_code: " << _redir_code << std::endl;
	std::cout << "Location redir_url: " << _redir_url << std::endl;
	std::cout << "Location upload_store: " << _upload_store << std::endl;
	std::cout << "Location cgi: ";
	for (std::map<std::string, std::string>::iterator it = _cgi.begin(); it != _cgi.end(); it++)
		std::cout << it->first << " " << it->second << " ";
	std::cout << std::endl;
	std::cout << "----------------" << std::endl;
}

