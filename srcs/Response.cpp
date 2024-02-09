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
		return (setErrorResponse(403));
	// Select behavior based on method of the request
	switch (_request.getMethod())
	{
		case get_:
			// if (checkCGI())
			// 	executeCGI();
			// else
				handleGetResource();
			break;
		case post_:
			// if (checkCGI())
			// 	executeCGI();
			// else
				handleFileUpload();
			break;
		case delete_:
			handleDeleteFile();
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
	std::vector<std::string>	loc_paths;
	std::vector<Location>		locations = _request.getLocations();
	for (int i = 0; i < locations.size(); i++)
		loc_paths.push_back(locations[i].getPath());

	
}
