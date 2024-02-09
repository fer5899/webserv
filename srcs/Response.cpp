#include "../include/Response.hpp"


Response::Response(Request &request) : _request(request)
{
	buildHttpResponse();
}

Response::~Response()
{
}

void	Response::buildHttpResponse()
{
	// Check if there was an error in the request syntax
	if (_request.getMethod() == 0)
		return (setErrorResponse(400));
	// Check if there is a location that matches the requested URL
	_location = matchLocation();
	if (!_location)
		return (setErrorResponse(404));
	// Check if the method of the request is allowed in the location
	if (!checkValidMethod())
		return (setErrorResponse(405));
	// Handle redirects
	if (_location->getRedirCode() > 0)
		return (setRedirection());
	// Select behavior based on method of the request
	switch (_request.getMethod())
	{
		case get_:
			// if (checkCGI())
			// 	executeCGI();
			// else
				return (handleGetResource());
			break;
		case post_:
			// if (checkCGI())
			// 	executeCGI();
			// else
				return (handleFileUpload());
			break;
		case delete_:
			return (handleDeleteFile());
			break;
		default:
			return (setErrorResponse(500));
	}
}

std::string	Response::getHttpResponse()
{
	return (_http_response);
}

Location	*Response::matchLocation()
{
	std::vector<Location>		locations = _request.getLocations();
	if (locations.empty())
		return (NULL);
	size_t	max_loc_size = 0;
	Location	*matched_loc;
	std::string	req_path = _request.getPath();
	std::string loc_path;

	for (size_t i = 0; i < locations.size(); i++)
	{
		loc_path = locations[i].getPath();
		if (loc_path.size() <= req_path.size())
		{
			if (req_path.compare(0, loc_path.size(), loc_path) == 0
				&& loc_path.size() > max_loc_size)
			{
				matched_loc = &locations[i];
				max_loc_size = loc_path.size();
			}
		}
	}
	if (max_loc_size)
		return (matched_loc);
	return (NULL);
}

bool	Response::checkValidMethod()
{
	std::vector<int> methods = _location->getMethods();

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (_request.getMethod() == methods[i])
			return (true);
	}
	return (false);
}

void	Response::handleGetResource()
{
	// Build filesystem path
	std::string file_path = _request.getPath();
	file_path = file_path.substr(_location->getPath().size(), std::string::npos);
	file_path = _location->getFilePathPrefix() + file_path;

	// Check if filepath points to a file or a directory
	struct stat fileStat;
	if (stat(file_path.c_str(), &fileStat) == 0)
	{
		// Read contents of the file into response body if it's a file
		if (S_ISREG(fileStat.st_mode))
		{
			std::ifstream stream(file_path);
			std::stringstream buffer;
			if (stream.is_open())
			{
				buffer << stream.rdbuf();
				_body = buffer.str();
			}
		}
		else if (S_ISDIR(fileStat.st_mode))
			return (handleGetDirectory());
		else
			return (setErrorResponse(500));
	}
	else
		return setErrorResponse(404);


	// Build http_response
	_status = "HTTP/1.1 200 OK\r\n";
	buildHeaders();
	_http_response = _status + _headers_str + "\r\n" + _body + "\r\n";
	
}
