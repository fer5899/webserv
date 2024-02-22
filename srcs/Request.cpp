#include "../include/Request.hpp"

Request::Request()
{
}

Request::Request(std::string path, std::map<std::string, std::string> headers, std::string body, std::string method, int error_code)
{
	_path = path;
	_headers = headers;
	_body = body;
	_method = method;
	_errorCode = error_code;
}

Request::~Request()
{
}

std::string	Request::getPath()
{
	return _path;
}

std::map<std::string, std::string>	Request::getHeaders()
{
	return _headers;
}

std::string	Request::getBody()
{
	return _body;
}

std::string	Request::getMethod()
{
	return _method;
}

int	Request::getErrorCode()
{
	return _errorCode;
}

std::map<std::string, std::string>	Request::getQueryParams()
{
	return _query_params;
}

