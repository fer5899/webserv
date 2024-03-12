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
	_cgi_path = std::vector<std::string>();
	_cgi_ext = std::vector<std::string>();
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
	_cgi_path = config.getCgiPath();
	_cgi_ext = config.getCgiExt();
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
	_cgi_path = other._cgi_path;
	_cgi_ext = other._cgi_ext;
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
		_cgi_path = other._cgi_path;
		_cgi_ext = other._cgi_ext;
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

std::vector<std::string >	Location::getCgiPath()
{
	return _cgi_path;
}

std::vector<std::string >	Location::getCgiExt()
{
	return _cgi_ext;
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

void	Location::setCgiPath(std::vector<std::string > cgi_path)
{
	_cgi_path = cgi_path;
}

void	Location::setCgiExt(std::vector<std::string > cgi_ext)
{
	_cgi_ext = cgi_ext;
}

