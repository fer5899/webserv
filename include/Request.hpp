

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "Client.hpp"

class Request
{
	private:
		std::string 						_raw_request;
		std::string							_path;
		std::map<std::string, std::string>	_query_params;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_method;
		int									_errorCode;

	public:
		Request();
		Request(std::string path, std::map<std::string, std::string> headers, std::string body, std::string method, int error_code); // Testing constructor
		~Request();

		std::vector<Location>				getLocations();
		int									getErrorCode();
		std::string							getMethod();
		std::map<std::string, std::string>	getHeaders();
		std::string							getBody();
		std::string							getPath();
		std::map<std::string, std::string>	getQueryParams();

};

#endif