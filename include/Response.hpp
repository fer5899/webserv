
#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <iostream>
# include <map>
# include <fstream>
# include <sstream>
# include <sys/time.h>
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
# include "Request.hpp"

class Request;

template <typename T>
std::string numberToString ( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

class Response
{
	private:
		std::string 						_status;
		std::string							_headers_str;
		std::string							_body;
		std::string							_http_response;
		Request								*_request;
		Client								&_client;
		Location							*_location;

		void		setContentType();
		void		setDateServer();
		void		setErrorResponse(int status);
		void		setRedirection();
		Location	*matchLocation();
		bool		checkValidMethod();
		bool		checkCGI();
		void		executeCGI();
		void		handleGetDirectory(std::string filesys_dir_path);
		void		handleGetResource(std::string filesys_path);
		void		handleFileUpload();
		void		handleDeleteFile();
		std::string	buildFilesystemPath(std::string request_path);
		void		buildAutoindex(std::string filesys_dir_path);
		void		buildSimpleErrorPage();
		void		buildStatus(int status);
		void		buildHttpResponse();

	public:
		Response(Client &client);
		~Response();

		std::string	getHttpResponse();

};

#endif



