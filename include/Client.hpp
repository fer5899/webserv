#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Request.hpp"

class Client
{
	public:
		Client(Server server, int socket);
		~Client();

		int getSocket() const;
		Request *getRequest() const;
		void setRequest(void);
		
	private:
		Server _server;
		int _socket;
		Request *_Request;
		//Response _response;
};

#endif