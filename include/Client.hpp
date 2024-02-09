
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <iostream>
# include <map>
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"

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

};

#endif



