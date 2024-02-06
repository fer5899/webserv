#include "../include/Server.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/select.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 100

int main()
{
	int server_socket;

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}

	// Forcefully attaching socket to the port 4242
	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(4242);

	if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		return 1;
	}

	if (listen(server_socket, 3) < 0)
	{
		std::cerr << "Listen failed" << std::endl;
		return 1;
	}
	
	std::cout << "Waiting for a connection on port 4242" << std::endl;

	//I create a vector of client sockets so I don't have to iterate over all the file descriptors (1024) to check if they are ready to read
	std::vector<int> client_sockets;

	fd_set read_sockets, write_sockets, error_sockets;

	int count = 0;
	while (1)
	{
		std::cout << "Count: " << count++ << "\n";
		FD_ZERO(&read_sockets);
		FD_ZERO(&write_sockets);
		FD_ZERO(&error_sockets);
		FD_SET(server_socket, &read_sockets);
		FD_SET(server_socket, &error_sockets);
		
		for (long unsigned int i = 0; i < client_sockets.size(); i++)
		{
			FD_SET(client_sockets[i], &write_sockets);
			FD_SET(client_sockets[i], &read_sockets);
		}

		int max_socket = server_socket;
		for (long unsigned int i = 0; i < client_sockets.size(); i++)
		{
			max_socket = std::max(max_socket, client_sockets[i]);
		}

		int	activity = select(max_socket + 1, &read_sockets, &write_sockets, &error_sockets, NULL);
		if (activity < 0)
		{
			std::cerr << "Select error" << std::endl;
			return 1;
		}

		//New connection
		if (FD_ISSET(server_socket, &read_sockets))
		{
			int new_socket;

			if ((new_socket = accept(server_socket, (struct sockaddr *)NULL, NULL)) < 0)
			{
				std::cerr << "Accept failed" << std::endl;
				return 1;
			}
			std::cout << "New connection, socket fd is " << new_socket << std::endl;
            client_sockets.push_back(new_socket);
		}
		else
		{
			//Read from client sockets
			std::vector<int> toDelete = client_sockets;
			for (long unsigned int i = 0; i < client_sockets.size(); i++)
			{
				int client_socket = client_sockets[i];

				if (FD_ISSET(client_socket, &read_sockets))
				{
					std::string httpRequest;

					while (true)
					{
						char buffer[BUFFER_SIZE];
						ssize_t bytesRead = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
						if (bytesRead == -1)
						{
							perror("recv");
							close(client_socket);
							toDelete.erase(toDelete.begin() + i);
							break;
						}
						else if (bytesRead == 0 && httpRequest.empty())
						{
							std::cout << "bytesRead == 0, closing connection" << std::endl;
							close(client_socket);
							toDelete.erase(toDelete.begin() + i);
							break;
						}
						else if (bytesRead < BUFFER_SIZE - 1) 
						{
							buffer[bytesRead] = '\0';
							httpRequest += buffer;
							std::cout << "Received complete HTTP Request:\n" << httpRequest << std::endl;
							break;
						}
						else 
						{
							buffer[bytesRead] = '\0';
							httpRequest += buffer;
						}
					}
				}

				//Write to client sockets
				else if (FD_ISSET(client_socket, &write_sockets))
				{
					//std::cout << "Write to client socket" << std::endl;
					const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
					ssize_t bytesSent = send(client_socket, response, strlen(response), 0);
					if (bytesSent == -1)
					{
						perror("send");
						close(client_socket);
						toDelete.erase(toDelete.begin() + i);
					}
					else if (bytesSent == 0)
					{
						close(client_socket);
						toDelete.erase(toDelete.begin() + i);
					}
					else 
					{
						std::cout << "Data sent" << std::endl;
						close(client_socket);
						toDelete.erase(toDelete.begin() + i);
						std::cout << "Connection closed" << std::endl;
					}
				}

				//Error on client sockets
				else if (FD_ISSET(client_socket, &error_sockets))
				{
					std::cerr << "Error on client socket" << std::endl;
					close(client_socket);
					toDelete.erase(toDelete.begin() + i);
				}
			}
			client_sockets = toDelete;
		}
	}
	return 0;
}