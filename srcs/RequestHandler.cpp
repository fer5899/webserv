#include "../include/RequestHandler.hpp"

RequestHandler::RequestHandler(/*Client &client*/) : _path(""), _method(""), _version(""), _headers(), _body(""), _errorCode(0), _buffer(""), _size(0), _bodySize(MAX_REQUEST_SIZE), _state(0)
{}

RequestHandler::~RequestHandler()
{}

RequestHandler& RequestHandler::operator=(const RequestHandler& other)
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
	}
	return *this;
}

RequestHandler::RequestHandler(const RequestHandler& other) : _path(other._path), _method(other._method), _version(other._version), _headers(other._headers), _body(other._body), _errorCode(other._errorCode), _buffer(other._buffer), _size(other._size), _bodySize(other._bodySize) , _state(other._state){}

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

bool RequestHandler::parseHeaders(std::string& line)
{
	if (line.empty() && _method == "POST")
	{
		return parseBodyRequisites();
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

bool RequestHandler::parseBodyRequisites()
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
//IMPORTANTE VER CUANTOS DE ESTOS ACEPTAMOS AL FINAL
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
			_bodySize = std::stod(_headers["Content-Length"]) + 1;
		}
		catch (std::exception &e)
		{
			_errorCode = 400;
			return true;
		}
	}
	_state = 2;
	return false;
}

bool RequestHandler::parseBody(std::string& line)
{
	_body += line;
	_bodySize -= line.size() - countNewlines(line);
	if (_bodySize < 0)
	{
		_errorCode = 413;
		return true;
	}
	return false;
}

bool RequestHandler::parseRequest(std::string& request)
{
	int request_size = request.size();
	if (request_size  == 0)
	{
		_errorCode = 400;
		return true;
	}
	_size += request_size - countNewlines(request);
	if (_size > MAX_REQUEST_SIZE)
	{
		_errorCode = 413;
		return true;
	}
	std::string::size_type pos = request.find("\n"); // Incluir salto de carro
	while (pos != std::string::npos && _state < 2)
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
		pos = request.find("\n"); // Incluir salto de carro
	}
	if (_state == 2)
	{
		if (parseBody(request))
			return true;
	}
	_buffer += request;
	return false;
}