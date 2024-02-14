#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
	public:
		Client(Server server, int socket);
		~Client();

		int getSocket() const;
		
	private:
		Server _server;
		int _socket;
		//Request *_request;
		//Response *_response;

};

#endif