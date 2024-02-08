# ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/select.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

class Server
{
	public:
		Server(int port);
		~Server();

		void setUpServer();
		int getSocket() const;

	private:
		int _port;
		int _server_socket;
		struct sockaddr_in _address;
};

#endif


