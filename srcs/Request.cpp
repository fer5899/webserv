#include "../include/Request.hpp"

Request::Request() : _path(""), _method(""), _version(""), _headers(), _body(""), _errorCode(0), _buffer(""), _size(0), _bodySize(MAX_REQUEST_SIZE), _state(0)
{
	_server_vector = NULL;
}
Request::Request(std::vector<Server> *server_vector) : _path(""), _method(""), _version(""), _headers(), _body(""), _errorCode(0), _buffer(""), _size(0), _bodySize(MAX_REQUEST_SIZE), _state(0)
{
	_server_vector = server_vector;
}

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
	std::string path = _path;
	size_t pos = path.find('?');
	if (pos != std::string::npos) {
		path = path.substr(0, pos);
	}
	return path;
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

std::string	Request::getHostname() const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find("Host");
	if (it == _headers.end())
		return "";
	if (it->second.find(':') != std::string::npos)
		return it->second.substr(0, it->second.find(':'));
	return it->second;
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
	if (_version != "HTTP/1.1")
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
		{
			_bodySize = getMaxBodySize();
			return parseBodyRequisites();
		}
		return true;
	}
	std::string::size_type pos = line.find(": ");
	if (pos == std::string::npos)
		return true;
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 2);
	_headers[key] = value;
	return false;
}

bool Request::parseBodyRequisites()
{
	if (_headers.find("Content-Type") == _headers.end())
	{
		_errorCode = 400;
		return true;
	}
	if (_headers.find("Content-Length") != _headers.end())
	{
		try
		{

			size_t bodySize = std::strtoul(_headers["Content-Length"].c_str(), 0, 10);
			if (bodySize < _bodySize)
				_bodySize = bodySize;
		}
		catch (std::exception &e)
		{
			_errorCode = 400;
			return true;
		}
	}
	_state = 2;
	if (_headers.find("Transfer-Encoding") != _headers.end() && _headers["Transfer-Encoding"] == "chunked")
		_state = 3;
	return false;
}

size_t Request::getMaxBodySize()
{
	for (std::vector<Server>::iterator it = _server_vector->begin(); it != _server_vector->end(); it++)
	{
		if (it->getServerName() == getHostname())
			return (*it).getMaxBodySize();
	}
	return _server_vector->begin()->getMaxBodySize();
}

bool Request::parseBody(std::string& line)
{
	if (line.size() > _bodySize)
	{	
		_body += line.substr(0, _bodySize);
		_bodySize = 0;
		_errorCode = 413;
		return true;
	}
	else
		_body += line;
	_bodySize -= line.size();
	if (_bodySize == 0)
		return true;
	return false;

}

bool	Request::parseBodyChunked(std::string line)
{
	line = _buffer + line;
	std::string::size_type pos = line.find("\r\n");
	while (pos != std::string::npos)
	{
		_buffer = line;
		std::string chunk = line.substr(0, pos);
		line.erase(0, pos + 2);
		if (chunk.empty())
			return true;
		try {
			size_t chunkSize = std::strtoul(chunk.c_str(), 0, 16);
			if (chunkSize == 0)
				return true;
			if (chunkSize > _bodySize)
			{
				_errorCode = 413;
				return true;
			}
			if (line.size() < chunkSize + 2)
				return false;
			_body += line.substr(0, chunkSize);
			line.erase(0, chunkSize + 2);
			if (_body.size() > _bodySize)
			{
				_errorCode = 413;
				return true;
			}
			else if (_body.size() == _bodySize)
				return true;
		} catch (std::exception &e) {
			_errorCode = 400;
			return true;
		}
		pos = line.find("\r\n");
	}
	_buffer = line;
	return false;
}
#include <limits>

bool Request::parseRequest(std::string request)
{
	int request_size = request.size();
	if (request_size == 0)
		return false;
	_size += request_size - countSubstring(request, "\n") - countSubstring(request, "\r");
	if (_size > MAX_REQUEST_SIZE)
	{
		_errorCode = 413;
		return true;
	}
	std::string::size_type pos = request.find("\n");
	while (pos != std::string::npos && _state < 2)
	{
		while (pos != std::numeric_limits<size_t>::max() && std::isspace(request[pos]))
			pos--;
		std::string line = _buffer + request.substr(0, pos + 1);
		size_t first = line.find_first_not_of(" \t\r\n");
		size_t last = line.find_last_not_of(" \t\r\n");
		if (first == std::string::npos)
			first = 0;
		if (last == std::string::npos)
			last = line.size() - 1;
		line = line.substr(first, last - first + 1);
		_buffer = "";
		pos = request.find("\n");
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
	else if (_state == 3)
	{
		if (parseBodyChunked(request))
			return true;
	}
	_buffer += request;
	return false;
}

std::ostream& operator<<(std::ostream& os, const Request& Request)
{
	if (Request.getErrorCode() != 0)
		os << RED "Error code detected in Request: " RESET << Request.getErrorCode() << std::endl;
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
		os << BLUE "Body: " RESET << std::endl;
		if (Request.getBody().size() > 300)
		{
			os << Request.getBody().substr(0, 300) << "..." << std::endl;
		}
		else
		{
			os << Request.getBody() << std::endl;
		}
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

