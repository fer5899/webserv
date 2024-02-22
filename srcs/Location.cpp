#include "../include/Location.hpp"

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


