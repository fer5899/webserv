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
	_cgi_path = "";
	_cgi_ext = "";
}

Location::Location(std::string path, std::string root, std::string alias, std::vector<std::string> methods, bool autoindex, std::string index, int redir_code, std::string redir_url, std::string upload_store, std::string cgi_path, std::string cgi_ext)
{
	_path = path;
	_root = root;
	_alias = alias;
	_methods = methods;
	_autoindex = autoindex;
	_index = index;
	_redir_code = redir_code;
	_redir_url = redir_url;
	_upload_store = upload_store;
	_cgi_path = cgi_path;
	_cgi_ext = cgi_ext;
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

std::string	Location::getCgiPath()
{
	return _cgi_path;
}

std::string	Location::getCgiExt()
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

void	Location::addMethod(std::string &method)
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

void	Location::setCgiPath(std::string cgi_path)
{
	_cgi_path = cgi_path;
}

void	Location::setCgiExt(std::string cgi_ext)
{
	_cgi_ext = cgi_ext;
}
