#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "common.hpp"

class Request
{
	private:
		//Info to send to the response
		std::string							_path;
		std::string							_method;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		int									_errorCode;

		std::map<std::string, std::string>	_query_params;


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
		std::string							getPath() const;
		std::string							getHTTPVersion() const;
		std::string							getBody() const;
		bool								keepAlive() const;
		int									getErrorCode() const;
		void								setErrorCode(int errorCode);
		std::map<std::string, std::string>	getQueryParams() const;

		bool 								parseRequest(std::string request);

		void								setMethod(std::string method);
		void								setHeaders(std::map<std::string, std::string> headers);
		void								setBody(std::string body);
		void								setPath(std::string path);
		void								setQueryParams(std::map<std::string, std::string> query_params);

};
std::ostream& operator<<(std::ostream& os, const Request& Request);

#endif

