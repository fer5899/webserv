

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
		std::string 										raw_request;
		std::string											url;
		std::map<std::string, std::string>					headers;
		std::vector<std::map<std::string, std::string> >	body;
		int													method;
		Client												&client;
		bool												bad_request;

	public:
		Request(/* args */);
		~Request();

		std::vector<Location>	getLocations();
		int						getMethod();
		std::map<std::string, std::string>	getHeaders();
		std::vector<std::map<std::string, std::string> >	getBody();
		std::string	getUrl();

};

#endif