#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "common.hpp"

class Request;

class Response;

class Client
{
	private:
		int			_socket;
		Server		*_server;
		Request		*_request;
		Response	*_response;
		time_t		_last_req_time;

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
		void 		clearRequest();
		void		clearResponse();

		void		setRequest(Request *request);
		void		setResponse(Response *response);
		void		setLastReqTime();
		const time_t	&getLastReqTime() const;

};

#endif



