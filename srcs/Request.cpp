#include "../include/Request.hpp"

Request::Request()
{
	_path = "";
	_headers = std::map<std::string, std::string>();
	_body = "";
	_method = "";
	_errorCode = 0;
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

void	Request::setPath(std::string path)
{
	_path = path;
}

void	Request::setHeaders(std::map<std::string, std::string> headers)
{
	_headers = headers;
}

void	Request::setBody(std::string body)
{
	_body = body;
}

void	Request::setMethod(std::string method)
{
	_method = method;
}

void	Request::setErrorCode(int error_code)
{
	_errorCode = error_code;
}

void	Request::setQueryParams(std::map<std::string, std::string> query_params)
{
	_query_params = query_params;
}

