#include <iostream>
#include <map>

class RequestHandler
{
	private:
		bool		_finished;
		std::string	_url;
		std::string	_method;
		std::string	_http_version;
		std::string	_body;
		std::map<std::string, std::string> _headers;
	public:
		RequestHandler();
		~RequestHandler();
		RequestHandler& operator=(const RequestHandler& other);
		RequestHandler(const RequestHandler& other);
		std::map<std::string, std::string> getHeaders() const;
		std::string getMethod() const;
		std::string getURL() const;
		std::string getHTTPVersion() const;
		std::string getBody() const;
		bool parseRequest(const std::string& request);
};


