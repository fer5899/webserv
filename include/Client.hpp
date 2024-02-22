
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <map>
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Request;

class Response;

class Client
{
	private:
		int			_socket;
		Server		&_server;
		Request		*_request;
		Response	*_response;
		int			_req_count;

	public:
		Client(int socket, Server &server);
		~Client();

		Request		*getRequest() const;
		Response	*getResponse() const;
		int			getSocket() const;
		Server		&getServer() const;

		void		setRequest(Request *request);

};

#endif



