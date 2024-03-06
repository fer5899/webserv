#include "../include/Response.hpp"


Response::Response(Client *client, Request *request) : _client(client), _request(request)
{
	_status = "";
	_headers_str = "";
	setDateServer();
	buildHttpResponse();
}

Response::Response(const Response &other)
{
	*this = other;
}

Response &Response::operator=(const Response &other)
{
	_status = other._status;
	_headers_str = other._headers_str;
	_body = other._body;
	_http_response = other._http_response;
	_client = other._client;
	_request = other._request;
	_location = other._location;
	return *this;
}

Response::~Response()
{
}

void	Response::buildHttpResponse()
{
	// Check if there was an error in the request parsing
	if (_request->getErrorCode() > 0)
		return (setErrorResponse(_request->getErrorCode()));
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
	if (_request->getMethod() == "GET")
	{
		// if (checkCGI())  Comprobar existencia de 2 values
		//     return executeCGI(); Meter error si ext no es .cgi
		// else
		return handleGetResource(buildFilesystemPath(_request->getPath()));
	}
	else if (_request->getMethod() == "POST")
	{
		// if (checkCGI())
		//     return executeCGI();
		// else
		return handleFileUpload();
	}
	else if (_request->getMethod() == "DELETE")
		return handleDeleteFile();
	else
		return setErrorResponse(500);

}

std::string Response::generateTimestamp()
{
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	std::string	separator = "";

	std::stringstream timestamp;
	timestamp << localTime->tm_year + 1900 << separator
				<< std::setw(2) << std::setfill('0') << localTime->tm_mon + 1 << separator
				<< std::setw(2) << std::setfill('0') << localTime->tm_mday << separator
				<< std::setw(2) << std::setfill('0') << localTime->tm_hour << separator
				<< std::setw(2) << std::setfill('0') << localTime->tm_min << separator
				<< std::setw(2) << std::setfill('0') << localTime->tm_sec;

	return timestamp.str();
}

std::string	&Response::getHttpResponse()
{
	return (_http_response);
}

Location	*Response::matchLocation()
{
	Server	*server = _client->getServer();
	std::vector<Location>		locations = server->getLocations();
	if (locations.size() == 0)
		return (NULL);
	size_t	max_loc_size = 0;
	Location	*matched_loc;
	std::string	req_path = _request->getPath();
	std::string loc_path;

	for (size_t i = 0; i < locations.size(); i++)
	{
		loc_path = _client->getServer()->getLocation(i).getPath();
		if (loc_path.size() <= req_path.size())
		{
			if (req_path.compare(0, loc_path.size(), loc_path) == 0
				&& loc_path.size() > max_loc_size)
			{
				matched_loc = &(_client->getServer()->getLocation(i));
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
	std::vector<std::string> methods = _location->getMethods();

	for (size_t i = 0; i < methods.size(); i++)
	{
		if (_request->getMethod() == methods[i])
			return (true);
	}
	return (false);
}

void	Response::handleGetResource(std::string filesys_path)
{
	// Check if filepath points to a file or a directory
	struct stat fileStat;
	if (stat(filesys_path.c_str(), &fileStat) == 0)
	{
		// Read contents of the file into response body if it's a file
		if (S_ISREG(fileStat.st_mode))
		{
			std::cout << "Filepath corresponds to a file" << std::endl;
			std::ifstream stream(filesys_path);
			std::stringstream buffer;
			if (stream.is_open())
			{
				buffer << stream.rdbuf();
				_body = buffer.str();
			}
		}
		else if (S_ISDIR(fileStat.st_mode))
			return (handleGetDirectory(filesys_path));
		else
			return (setErrorResponse(500));
	}
	else
		return setErrorResponse(404);


	// Build http_response
	_status = "HTTP/1.1 200 OK\r\n";
	setContentType(filesys_path);
	_headers_str.append("Content-Length: " + numberToString(_body.size()) + "\r\n");
	_http_response = _status + _headers_str + "\r\n" + _body + "\r\n";
	
}

void	Response::setDateServer()
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;

	gettimeofday(&tv, NULL);
	tm = localtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	std::string date = std::string(buffer);

	_headers_str.append("Server: Webserv42\r\n");
	_headers_str.append("Date: " + date + "\r\n");

}

void	Response::setContentType(std::string filesys_path)
{
	std::map<std::string, std::string> content_types;
	content_types["html"] = "text/html"; 
	content_types["css"] = "text/css";
	content_types["js"] = "text/javascript";
	content_types["jpg"] = "image/jpeg";
	content_types["jpeg"] = "image/jpeg";
	content_types["png"] = "image/png";
	content_types["gif"] = "image/gif";
	content_types["bmp"] = "image/bmp";
	content_types["ico"] = "image/x-icon";
	content_types["svg"] = "image/svg+xml";
	content_types["mp3"] = "audio/mpeg";
	content_types["wav"] = "audio/wav";
	content_types["mp4"] = "video/mp4";
	content_types["avi"] = "video/x-msvideo";
	content_types["mpeg"] = "video/mpeg";
	content_types["txt"] = "text/plain";
	content_types["pdf"] = "application/pdf";
	content_types["zip"] = "application/zip";
	content_types["tar"] = "application/x-tar";
	content_types["gz"] = "application/x-gzip";
	content_types["rar"] = "application/x-rar-compressed";
	content_types["7z"] = "application/x-7z-compressed";
	content_types["exe"] = "application/x-msdownload";
	content_types["json"] = "application/json";
	content_types["xml"] = "application/xml";
	content_types["csv"] = "text/csv";

	std::string		content_type;
	size_t			dot_pos = filesys_path.find_last_of('.');
	if (dot_pos != std::string::npos)
	{
		std::string	extension = filesys_path.substr(dot_pos + 1);
		try
		{
			content_type = content_types.at(extension);
		}
		catch(const std::out_of_range& e)
		{
			content_type = "application/octet-stream";
		}
	}
	else
		content_type = "application/octet-stream";
	_headers_str.append("Content-Type: " + content_type + "\r\n");
}

void	Response::buildStatus(int status)
{
	std::map<int, std::string> status_lines;
	status_lines[200] = "200 OK";
	status_lines[201] = "201 Created";
	status_lines[204] = "204 No Content";
	status_lines[300] = "300 Multiple Choices";
	status_lines[301] = "301 Moved Permanently";
	status_lines[302] = "302 Found";
	status_lines[303] = "303 See Other";
	status_lines[304] = "304 Not Modified";
	status_lines[307] = "307 Temporary Redirect";
	status_lines[308] = "308 Permanent Redirect";
	status_lines[400] = "400 Bad Request";
	status_lines[401] = "401 Unauthorized";
	status_lines[403] = "403 Forbidden";
	status_lines[404] = "404 Not Found";
	status_lines[405] = "405 Method Not Allowed";
	status_lines[408] = "408 Request Timeout";
	status_lines[413] = "413 Payload Too Large";
	status_lines[415] = "415 Unsupported Media Type";
	status_lines[500] = "500 Internal Server Error";
	status_lines[501] = "501 Not Implemented";
	status_lines[502] = "502 Bad Gateway";
	status_lines[503] = "503 Service Unavailable";
	status_lines[505] = "505 HTTP Version Not Supported";

	try
	{
		_status = "HTTP/1.1 " + status_lines.at(status) + "\r\n";
	}
	catch(const std::out_of_range& e)
	{
		_status = "HTTP/1.1 500 Internal Server Error\r\n";
	}	
}

void	Response::setRedirection()
{
	buildStatus(_location->getRedirCode());
	_headers_str.append("Location: " + _location->getRedirUrl() + "\r\n");
	_http_response = _status + _headers_str + "\r\n";
}

void	Response::buildSimpleErrorPage()
{
	_body = "<html><head><title>" + _status + "</title></head><body><h1>" + _status + "</h1></body></html>";
}

void	Response::setErrorResponse(int errorCode)
{
	buildStatus(errorCode);
	// Get error page from default error pages
	try
	{
		std::string error_page_path = _client->getServer()->getDefError().at(errorCode);
		struct stat fileStat;
		if (stat(error_page_path.c_str(), &fileStat) == 0)
		{
			// Read contents of the file into response body if it's a file
			if (S_ISREG(fileStat.st_mode))
			{
				std::ifstream stream(error_page_path);
				std::stringstream buffer;
				if (stream.is_open())
				{
					buffer << stream.rdbuf();
					_body = buffer.str();
				}
			}
			else
				buildSimpleErrorPage();
		}
		else
			buildSimpleErrorPage();
	}
	catch(const std::out_of_range& e)
	{
		// If no default error page was defined in the configuration, build a simple error page
		buildSimpleErrorPage();
	}
	
	_headers_str.append("Content-Type: text/html\r\n");
	_headers_str.append("Content-Length: " + numberToString(_body.size()) + "\r\n");
	_http_response = _status + _headers_str + "\r\n" + _body + "\r\n";
}

std::string	Response::buildFilesystemPath(std::string request_path)
{
	std::string filesystem_path;
	std::string	root = _location->getRoot();
	std::string alias = _location->getAlias();

	if (!alias.empty())
	{
		filesystem_path = request_path.substr(_location->getPath().size(), std::string::npos);
		filesystem_path = alias + filesystem_path;
	}
	else
		filesystem_path = root + request_path;

	return ("." + filesystem_path);
}

void	Response::handleGetDirectory(std::string filesys_dir_path)
{
	// Check if the location has an index defined
	std::string filesys_index_path = filesys_dir_path + "/" + _location->getIndex();
	std::cout << "Index path: " << filesys_index_path << std::endl;
	struct stat fileStat;
	if (access(filesys_index_path.c_str(), F_OK) == 0
		&& stat(filesys_index_path.c_str(), &fileStat) == 0 
		&& S_ISREG(fileStat.st_mode))
	{
		std::cout << "Index exists" << std::endl;
		if (access(filesys_index_path.c_str(), R_OK) == 0)
			return handleGetResource(filesys_index_path);
		else
			return setErrorResponse(403);
	}
	// Check if the location has autoindex activated
	if (_location->getAutoindex()) // If it does, build simple autoindex
		return buildAutoindex(filesys_dir_path);
	else // If no index can be found and autoindex is deactivated, set error 404
		return setErrorResponse(404);
}

void	Response::buildAutoindex(std::string filesys_dir_path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(filesys_dir_path.c_str())) != NULL)
	{
		_body = "<html><head><title>Index of " + _request->getPath() + "</title></head><body><h1>Index of " + _request->getPath() + "</h1><hr><pre>";
		while ((ent = readdir(dir)) != NULL)
		{
			_body.append("<a href=\"" + _request->getPath() + "/" + ent->d_name + "\">" + ent->d_name + "</a><br>");
		}
		buildStatus(200);
		_headers_str.append("Content-Type: text/html\r\n");
		_headers_str.append("Content-Length: " + numberToString(_body.size()) + "\r\n");
		_body.append("</pre><hr></body></html>");
		_http_response = _status + _headers_str + "\r\n" + _body + "\r\n";
		closedir(dir);
	}
	else
		return setErrorResponse(500);
}

void	Response::parseUploadBody(std::string body, std::string boundary, std::vector<std::string> &form_elements_filenames, std::vector<std::string> &form_elements_contents)
{
	std::string	section;
	std::string scd; // Section content disposition
	std::string filename;
	std::string s_body; // Section body
	size_t start_pos = 0;
	size_t end_pos = 0;

	while ((end_pos = body.find("--" + boundary, start_pos)) != std::string::npos)
	{
		section = body.substr(start_pos, end_pos - start_pos);
		scd = section.substr(0, section.find("\r\n", 2));
		size_t body_start = section.find("\r\n\r\n") + 4;
		size_t filename_pos = scd.find("filename=");
		if (filename_pos != std::string::npos)
		{
			if (section.size() > body_start + 2)
			{
				// Parse filename from Content-Disposition
				filename = scd.substr(filename_pos + 9, scd.find_first_of(";\r", filename_pos + 9) - filename_pos - 9);
				// Trim the filename of leading and trailing whitespaces and quotes
				size_t start = filename.find_first_not_of(" \"");
				size_t end = filename.find_last_not_of(" \"");
				filename = filename.substr(start, end - start + 1);
				// Get the body of the section
				s_body = section.substr(body_start, section.size() - body_start - 2);
				// Store the filename and body
				form_elements_filenames.push_back(filename);
				form_elements_contents.push_back(s_body);
			}
		}
		start_pos = end_pos + boundary.size() + 2;
	}
}

void	Response::handleFileUpload()
{
	// Check if the location allows file uploads
	std::string upload_store = _location->getUploadStore();
	if (upload_store.empty())
		return setErrorResponse(403);

	std::string content_type;
	// Check if Content-Type header in request is multipart/form-data or multipart/mixed
	try
	{
		content_type = _request->getHeaders().at("Content-Type");
		if (content_type.find("multipart/form-data") == std::string::npos
			&& content_type.find("multipart/mixed") == std::string::npos)
		{
			return setErrorResponse(400);
		}
	}
	catch(const std::out_of_range& e)
	{
		return setErrorResponse(400);
	}

	std::vector<std::string> form_elements_filenames;
	std::vector<std::string> form_elements_contents;

	// Parse boundary from Content-Type header
	std::string boundary = content_type.substr(content_type.find("boundary=") + 9);

	parseUploadBody(_request->getBody(), boundary, form_elements_filenames, form_elements_contents);

	// Create files with their respective filename and content
	for (size_t i = 0; i < form_elements_filenames.size(); i++)
	{
		std::string file_path = upload_store + "/" + generateTimestamp() + "_" + form_elements_filenames[i];
		std::ofstream file(file_path);
		if (file.is_open())
		{
			file << form_elements_contents[i];
			file.close();
		}
		else
		{
			return setErrorResponse(500);
		}
	}

	// Build response
	_status = "HTTP/1.1 201 Created\r\n";
	_headers_str.append("Content-Length: 0\r\n");
	_http_response = _status + _headers_str + "\r\n";
}

void	Response::handleDeleteFile()
{
	std::string file_path = buildFilesystemPath(_request->getPath());
	if (remove(file_path.c_str()) != 0)
		return setErrorResponse(404);

	// Build response
	_status = "HTTP/1.1 204 No Content\r\n";
	_headers_str.append("Content-Length: 0\r\n");
	_http_response = _status + _headers_str + "\r\n";
}

