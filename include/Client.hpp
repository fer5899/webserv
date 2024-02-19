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
		RequestHandler *getRequestHandler() const;
		void setRequestHandler(void);
		
	private:
		Server _server;
		int _socket;
		RequestHandler *_requestHandler;
		//Response _response;
};

#endif