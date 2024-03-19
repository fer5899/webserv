#include "../include/Request.hpp"

Request::Request() : _path(""), _method(""), _version(""), _headers(), _body(""), _errorCode(0), _buffer(""), _size(0), _bodySize(MAX_REQUEST_SIZE), _state(0)
{}
Request::Request(size_t maxBodySize) : _path(""), _method(""), _version(""), _headers(), _body(""), _errorCode(0), _buffer(""), _size(0), _bodySize(maxBodySize), _state(0)
{}

Request::~Request()
{}

Request& Request::operator=(const Request& other)
{
	if (this != &other)
	{
		_path = other._path;
		_method = other._method;
		_version = other._version;
		_headers = other._headers;
		_body = other._body;
		_errorCode = other._errorCode;
		_buffer = other._buffer;
		_size = other._size;
		_bodySize = other._bodySize;
		_state = other._state;
		_query_params = other._query_params;
	}
	return *this;
}

Request::Request(const Request& other)
{
	*this = other;
}

std::map<std::string, std::string>	Request::getHeaders() const
{
	return _headers;
}

std::string	Request::getMethod() const
{
	return _method;
}

std::string	Request::getPath() const
{
	return _path;
}

std::string	Request::getHTTPVersion() const
{
	return _version;
}

std::string	Request::getBody() const
{
	return _body;
}

int		Request::getErrorCode() const
{
	return _errorCode;
}

bool	Request::keepAlive() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Connection");
	return (it != _headers.end() && it->second == "keep-alive");
}

bool Request::parseFirstLine(std::string& line)
{
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens.size() != 3)
	{
		_errorCode = 400;
		return true;
	}
	_method = tokens[0];
	_path = tokens[1];
	_version = tokens[2];
	if (_method != "GET" && _method != "POST" && _method != "DELETE")
	{
		std::cerr << "Error code: " << _errorCode << " "<<std::endl;
		_errorCode = 405;
		return true;
	}
	else if (_version != "HTTP/1.1")
	{
		_errorCode = 505;
		return true;
	}
	_state = 1;
	return false;
}

bool Request::parseHeaders(std::string& line)
{
	if (line.empty())
	{
		if (_method == "POST")
			return parseBodyRequisites();
		return true;
	}
	std::string::size_type pos = line.find(": ");
	if (pos == std::string::npos)
	{
		if (!line.empty())
			_errorCode = 400;
		return true;
	}
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 2);
	_headers[key] = value;
	return false;
}

bool Request::parseBodyRequisites()
{
	if (_headers.find("Content-Type") == _headers.end())
	{
		_errorCode = 415;
		return true;
	}
	std::vector<std::string> validTypes;
	validTypes.push_back("application/x-www-form-urlencoded");
	validTypes.push_back("multipart/form-data");
	validTypes.push_back("text/plain");
	validTypes.push_back("application/json");
	validTypes.push_back("application/xml");
	validTypes.push_back("application/octet-stream");
	validTypes.push_back("text/html");
	validTypes.push_back("image/jpeg");
	validTypes.push_back("image/png");
	validTypes.push_back("audio/mpeg");
	validTypes.push_back("application/pdf");
	validTypes.push_back("text/css");
	validTypes.push_back("application/javascript");
	for (std::vector<std::string>::iterator it = validTypes.begin(); it != validTypes.end(); it++)
	{
		if (_headers["Content-Type"].find(*it) != std::string::npos)
			break;
		if (it == validTypes.end() - 1)
		{
			_errorCode = 415;
			return true;
		}
	}
	if (_headers.find("Content-Length") != _headers.end())
	{
	try {
		size_t bodySize = std::stoul(_headers["Content-Length"]);
		if (bodySize < _bodySize)
			_bodySize = bodySize;
	} catch (std::exception &e) {
		_errorCode = 400;
		return true;
	}
	}
	_state = 2;
	return false;
}

bool Request::parseBody(std::string& line)
{
	_body += line;
	_bodySize -= line.size();
	if (_bodySize < 0)
	{
		_errorCode = 413;
		return true;
	}
	else if (_bodySize == 0)
	{
		return true;
	}
	return false;
}

bool Request::parseRequest(std::string request)
{
	int request_size = request.size();
	if (request_size  == 0)
	{
		_errorCode = 400;
		return true;
	}
	_size += request_size - countSubstring(request, "\n") - countSubstring(request, "\r");
	if (_size > MAX_REQUEST_SIZE)
	{
		_errorCode = 413;
		return true;
	}
	std::string::size_type pos = request.find("\n");
	while (pos != std::string::npos && _state < 2)
	{
		if (pos > 0 && request[pos - 1] == '\r')
			pos--;
		std::string line = _buffer + request.substr(0, pos);
		_buffer = "";
		if (request[pos] == '\r')
			request.erase(0, pos + 2);
		else
			request.erase(0, pos + 1);
		if (_state == 0)
		{
			if (parseFirstLine(line))
				return true;
		}
		else if (_state == 1)
		{
			if (parseHeaders(line))
				return true;
		}
		pos = request.find("\n");
	}
	if (_state == 2)
	{
		if (parseBody(request))
			return true;
	}
	_buffer += request;
	return false;
}

std::ostream& operator<<(std::ostream& os, const Request& Request)
{
	if (Request.getErrorCode() != 0)
	{
		os << RED "Error code: " RESET << Request.getErrorCode() << std::endl;
		return os;
	}
	os << "--------------------------------" << std::endl;
	os << BLUE "Method: " RESET << Request.getMethod() << std::endl;
	os << BLUE "URL: " RESET << Request.getPath() << std::endl;
	os << BLUE "HTTP version: " RESET << Request.getHTTPVersion() << std::endl;
	os << BLUE "Keep-Alive: " RESET << (Request.keepAlive() ? "true" : "false") << std::endl;
	os << "--------------------------------" << std::endl;
	os << BLUE "Headers:" RESET << std::endl;
	std::map<std::string, std::string> headers = Request.getHeaders();
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		os << CYAN << it->first << ": " RESET << it->second << std::endl;
	}
	if (Request.getBody().size() > 0)
	{
		os << "--------------------------------" << std::endl;
		os << BLUE "Body: " RESET << std::endl << Request.getBody() << std::endl;
	}
	os << "--------------------------------" << std::endl;
	return os;
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

