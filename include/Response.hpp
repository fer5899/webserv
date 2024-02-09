
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <map>
# include "Request.hpp"

class Response
{
	private:
		std::string 						_status;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_http_response;
		Request								&_request;
		Location							*_location;

		void		setErrorResponse(int status);
		Location	*matchLocation();
		bool		checkValidMethod();
		bool		checkCGI();
		void		executeCGI();
		void		handleGetResource();
		void		handleFileUpload();
		void		handleDeleteFile();
		void		buildHttpResponse();

	public:
		Response(Request &request);
		~Response();

		std::string	getHttpResponse();

};

#endif



