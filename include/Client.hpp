#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Request;

class Response;

class Client
{
	private:
		int			_socket;
		Server		*_server;
		Request		*_request;
		Response	*_response;
		// int			_req_count;

	public:
		Client(Server *server, int socket);
		Client(const Client &other);
		Client &operator=(const Client &other);
		~Client();

		Request		*getRequest();
		Response	*getResponse();
		int			getSocket() const;
		Server		*getServer();

		void		setRequest(Request *request);
		void		setResponse(Response *response);

};

#endif



