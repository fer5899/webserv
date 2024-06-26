
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
# include "Client.hpp"

class Request;
class Client;

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
		std::string							_req_path;
		Client								*_client;
		Request								*_request;
		Location							*_location;

		std::string							generateTimestamp();
		void								parseUploadBody(std::string body, std::string boundary, std::vector<std::string> &form_elements_filenames, std::vector<std::string> &form_elements_contents);
		Location							*matchLocation();
		void								setContentType(std::string filesys_path);
		void								setDateServer();
		void								setCookies();
		void								setErrorResponse(int status);
		void								setRedirection();
		bool								checkValidMethod();
		bool								checkCGI();
		void								executeCGI();
		void								handleGetDirectory(std::string filesys_dir_path);
		void								handleGetResource(std::string filesys_path);
		void								handleFileUpload();
		void								handleDeleteFile();
		std::string							buildFilesystemPath(std::string request_path);
		void								buildAutoindex(std::string filesys_dir_path);
		void								buildSimpleErrorPage();
		void								buildStatus(int status);
		void								buildHttpResponse();
		bool								isCGI();
		void								handleCGI();
		std::vector<std::string>			getCGIEnv();
		std::string							getCGICmd();

	public:
		Response(Client *client, Request *request);
		Response(const Response &other);
		Response &operator=(const Response &other);
		~Response();

		std::string	&getHttpResponse();
		bool		keepAlive();

		std::string	getStatus() const;
		std::string	getBody() const;
		std::map<std::string, std::string>	getHeaders() const;

};

std::ostream& operator<<(std::ostream& os, const Response& response);
#endif



