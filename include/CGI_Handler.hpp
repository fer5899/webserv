#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "common.hpp"
#include "Request.hpp"

class CGI_Handler {
	public:
		CGI_Handler();
		~CGI_Handler();
		CGI_Handler(const CGI_Handler& other);
		CGI_Handler& operator=(const CGI_Handler& other);

		std::string executeCGI(Request& request, std::string& body);
	private:
		std::map <std::string, std::string>	_env;
		std::string							_body;
};

#endif