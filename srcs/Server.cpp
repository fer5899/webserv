#include "../include/Server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 100

int main()
{
    int server_fd;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(8080);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Bind failed" << std::endl;
		return 1;
	}

	if (listen(server_fd, 3) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		return 1;
	}
	
	std::cout << "Waiting for a connection on port 8080" << std::endl;
	
	while (1)
	{
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(server_fd, &read_fds);

      if(select(server_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
	   {
		   std::cerr << "Select failed" << std::endl;
		   return 1;
	   }

    	if (FD_ISSET(server_fd, &read_fds))
      {
		
			int new_socket;

			if ((new_socket = accept(server_fd, (struct sockaddr *)NULL, NULL)) < 0)
			{
				std::cerr << "Accept failed" << std::endl;
				return 1;
			}

			std::string httpRequest;

			while (true) 
			{
				char buffer[BUFFER_SIZE];
				ssize_t bytesRead = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
				if (bytesRead == -1)
				{
					std::cerr << "Error reading from socket" << std::endl;
					close(new_socket);
					break;
				}

				buffer[bytesRead] = '\0';
				httpRequest += buffer;

				if (bytesRead < BUFFER_SIZE - 1) {
					std::cout << "Received complete HTTP Request:\n" << httpRequest << std::endl;

					const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
					send(new_socket, response, strlen(response), 0);

					break;
				}

				close(new_socket);
			}
		}
	}
    return 0;
}