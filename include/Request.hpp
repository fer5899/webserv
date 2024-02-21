#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "utils.hpp"

#define MAX_REQUEST_SIZE 1000000

//COLORS
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

class Request
{
	private:
		//Info to send to the response
		std::string							_path;
		std::string							_method;
		std::string							_version;
		std::map<std::string, std::string> _headers;
		std::string							_body;
		int									_errorCode;


		//Self info
		std::string							_buffer;
		size_t								_size;
		double								_bodySize;
		int									_state;

		bool 								parseFirstLine(std::string& line);
		bool								parseHeaders(std::string& line);
		bool								parseBodyRequisites();
		bool								parseBody(std::string& line);

	public:
		Request();
		~Request();
		Request(const Request& other);
		Request& operator=(const Request& other);

		std::map<std::string, std::string>	getHeaders() const;
		std::string							getMethod() const;
		std::string							getURL() const;
		std::string							getHTTPVersion() const;
		std::string							getBody() const;
		bool								keepAlive() const;
		int									getErrorCode() const;
		void								setErrorCode(int errorCode);

		bool 								parseRequest(std::string& request);
};
std::ostream& operator<<(std::ostream& os, const Request& Request);
