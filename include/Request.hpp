

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "Client.hpp"

enum e_request_methods
{
	error_,
	get_,
	post_,
	delete_,
};

class Request
{
	private:
		std::string 										_raw_request;
		std::string											_path;
		std::map<std::string, std::string>					_query_params;
		std::map<std::string, std::string>					_headers;
		std::vector<std::map<std::string, std::string> >	_body;
		std::string											_method;
		int													_errorCode;

	public:
		Request(/* args */);
		~Request();

		std::vector<Location>	getLocations();
		int						getErrorCode();
		std::string	getMethod();
		std::map<std::string, std::string>	getHeaders();
		std::vector<std::map<std::string, std::string> >	getBody();
		std::string	getPath();
		std::string	getQueryParams();

};

#endif