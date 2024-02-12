#include "../include/RequestHandler.hpp"

RequestHandler::RequestHandler(/*Client &client*/) : _path(""), _method(""), _version(""), _headers(), _errorCode(0), _body(""), _buffer(""), _size(0), _state(0)
{
	//_client = client;
}

RequestHandler::~RequestHandler()
{
	// free client
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other)
{
	if (this != &other)
	{
		_path = other._path;
		_method = other._method;
		_version = other._version;
		_headers = other._headers;
		_errorCode = other._errorCode;
		_body = other._body;
		_buffer = other._buffer;
		_size = other._size;
		_state = other._state;
	}
	return *this;
}

RequestHandler::RequestHandler(const RequestHandler& other) : _path(other._path), _method(other._method), _version(other._version), _headers(other._headers), _errorCode(other._errorCode), _body(other._body), _buffer(other._buffer), _size(other._size), _state(other._state){}

std::map<std::string, std::string>	RequestHandler::getHeaders() const
{
	return _headers;
}

std::string	RequestHandler::getMethod() const
{
	return _method;
}

std::string	RequestHandler::getURL() const
{
	return _path;
}

std::string	RequestHandler::getHTTPVersion() const
{
	return _version;
}

std::string	RequestHandler::getBody() const
{
	return _body;
}

void	RequestHandler::setErrorCode(int errorCode)
{
	_errorCode = errorCode;
}

int		RequestHandler::getErrorCode() const
{
	return _errorCode;
}

bool RequestHandler::parseFirstLine(std::string& line)
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
	if (_method != "GET" && _method != "POST" && _method != "DELETE" && _method != "PUT")
	{
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

bool RequestHandler::parseHeaders(std::string& line)
{
	if (line.empty())
	{
		_state = 2;
		return false;
	}
	std::string::size_type pos = line.find(": ");
	if (pos == std::string::npos)
	{
		_errorCode = 400;
		return true;
	}
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos + 2);
	_headers[key] = value;
	return false;
}

bool RequestHandler::parseRequest(std::string& request)
{
	int request_size = request.size();
	if (request_size == 0)
	{
		_errorCode = 400;
		return true;
	}
	_size += request_size;
	if (_size > MAX_REQUEST_SIZE)
	{
		_errorCode = 413;
		return true;
	}
	std::string::size_type pos = request.find("\n"); // Incluir salto de carro
	while (pos != std::string::npos)
	{
		
		std::string line = _buffer + request.substr(0, pos);
		_buffer = "";
		request.erase(0, pos + 2);
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
		else if (_state == 2)
		{
			_body += line;
			_body += "\n";
		}
		pos = request.find("\n"); // Incluir salto de carro
	}
	_buffer += request;
	return false;
}