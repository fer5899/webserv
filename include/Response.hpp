
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <map>
# include <fstream>
# include <sstream>
# include <sys/stat.h>
# include "Request.hpp"

class Request;

class Response
{
	private:
		std::string 						_status;
		std::map<std::string, std::string>	_headers;
		std::string							_headers_str;
		std::string							_body;
		std::string							_http_response;
		Request								&_request;
		Location							*_location;

		void		setErrorResponse(int status);
		void		setRedirection();
		Location	*matchLocation();
		bool		checkValidMethod();
		bool		checkCGI();
		void		executeCGI();
		void		handleGetDirectory();
		void		handleGetResource();
		void		handleFileUpload();
		void		handleDeleteFile();
		void		buildHeaders();
		void		buildHttpResponse();

	public:
		Response(Request &request);
		~Response();

		std::string	getHttpResponse();

};

#endif



