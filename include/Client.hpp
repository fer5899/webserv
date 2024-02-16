
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
		int			socket;
		Server		&server;
		Request		*request;
		Response	*response;
		int			req_count;

	public:
		Client(/* args */);
		~Client();

		Request		*getRequest() const;
		Response	*getResponse() const;
		int			getSocket() const;
		Server		&getServer() const;

};

#endif



