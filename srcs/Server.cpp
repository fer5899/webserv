#include "../include/Server.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/select.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>


#define BUFFER_SIZE 100

// Second version of the server

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

		
	int flags = fcntl(server_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);	// Add non-blocking flag and set close-on-exec flag, how the subject says
	if (flags == -1)
	{
		std::cerr << "Fcntl error" << std::endl;
		return 1;
	}
	// flags = fcntl(server_socket, F_SETFD, FD_CLOEXEC);	// Set close-on-exec flag, how it should be
	// if (flags == -1)
	// {
	// 	std::cerr << "Fcntl error" << std::endl;
	// 	return 1;
	// }


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

	
	fd_set read_sockets, write_sockets, error_sockets;
	fd_set read_sockets_copy, write_sockets_copy, error_sockets_copy;

	FD_ZERO(&read_sockets);
	FD_ZERO(&write_sockets);
	FD_ZERO(&error_sockets);
	FD_SET(server_socket, &read_sockets);
	FD_SET(server_socket, &error_sockets);	

	int max_socket = server_socket;

	int count = 0;
	while (1)
	{
		std::cout << "Count: " << count++ << "\n";
		read_sockets_copy = read_sockets;
		write_sockets_copy = write_sockets;
		error_sockets_copy = error_sockets;

		int	activity = select(max_socket + 1, &read_sockets_copy, &write_sockets_copy, &error_sockets_copy, NULL);
		if (activity < 0)
		{
			std::cerr << "Select error" << std::endl;
			return 1;
		}

		//New connection
		if (FD_ISSET(server_socket, &read_sockets_copy))
		{
			int new_socket;

			if ((new_socket = accept(server_socket, (struct sockaddr *)NULL, NULL)) < 0)
			{
				std::cerr << "Accept failed" << std::endl;
				return 1;
			}
			// std::cout << "New connection, socket fd is " << new_socket << std::endl;
			FD_SET(new_socket, &read_sockets);
			if (new_socket > max_socket)
				max_socket = new_socket;
		}
		else
		{
			//Read from client sockets
			for (int i = 0; i <= max_socket; i++)
			{
				if (FD_ISSET(i, &read_sockets_copy))
				{
					std::string httpRequest;

					// Right now it is a loop for example porpuses
					/* I should read the request on chunks and send it to the 
						httpresponse parser, and when it tells me that the 
						request is complete, I should could the connectionm, 
						build the response and change the fd from read to write */
					while (true)
					{
						char buffer[BUFFER_SIZE];
						ssize_t bytesRead = recv(i, buffer, BUFFER_SIZE - 1, 0);
						if (bytesRead == -1)
						{
							perror("recv");
							close(i);
							FD_CLR(i, &read_sockets);
							break;
						}
						else if (bytesRead == 0 && httpRequest.empty())
						{
							std::cout << "bytesRead == 0, closing connection" << std::endl;
							close(i);
							FD_CLR(i, &read_sockets);
							break;
						}
						else if (bytesRead < BUFFER_SIZE - 1) 
						{
							buffer[bytesRead] = '\0';
							httpRequest += buffer;
							std::cout << "Received complete HTTP Request:\n" << httpRequest << std::endl;
							FD_CLR(i, &read_sockets);
							FD_SET(i, &write_sockets);
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
				else if (FD_ISSET(i, &write_sockets_copy))
				{
					//std::cout << "Write to client socket" << std::endl;
					const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
					ssize_t bytesSent = send(i, response, strlen(response), 0);
					if (bytesSent == -1)
					{
						perror("send");
						close(i);
						FD_CLR(i, &write_sockets);
					}
					else if (bytesSent == 0)
					{
						close(i);
						FD_CLR(i, &write_sockets);
					}
					else 
					{
						std::cout << "Data sent" << std::endl;
						close(i);
						FD_CLR(i, &write_sockets);
						// FD_SET(i, &read_sockets);
						std::cout << "Connection closed" << std::endl;
					}
				}

				//Error on client sockets
				// right now I'm not sure when and where I should add the socket to the error set
				else if (FD_ISSET(i, &error_sockets_copy))
				{
					std::cerr << "Error on client socket" << std::endl;
					close(i);
					FD_CLR(i, &error_sockets);
				}
			}
		}
	}
	return 0;
}
			

// First version of the server, 1 client vector and I pass them all to the read and write sets each tiime

// int main()
// {
// 	int server_socket;

// 	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
// 	{
// 		std::cerr << "Socket creation error" << std::endl;
// 		return 1;
// 	}

// 	// Forcefully attaching socket to the port 4242
// 	int opt = 1;
// 	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// 	struct sockaddr_in address;
// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = htonl(INADDR_ANY);
// 	address.sin_port = htons(4242);

// 	if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
// 	{
// 		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
// 		return 1;
// 	}

// 	if (listen(server_socket, 3) < 0)
// 	{
// 		std::cerr << "Listen failed" << std::endl;
// 		return 1;
// 	}
	
// 	std::cout << "Waiting for a connection on port 4242" << std::endl;

// 	//I create a vector of client sockets so I don't have to iterate over all the file descriptors (1024) to check if they are ready to read
// 	std::vector<int> client_sockets;

// 	fd_set read_sockets, write_sockets, error_sockets;

// 	int count = 0;
// 	while (1)
// 	{
// 		std::cout << "Count: " << count++ << "\n";
// 		FD_ZERO(&read_sockets);
// 		FD_ZERO(&write_sockets);
// 		FD_ZERO(&error_sockets);
// 		FD_SET(server_socket, &read_sockets);
// 		FD_SET(server_socket, &error_sockets);
		
// 		for (long unsigned int i = 0; i < client_sockets.size(); i++)
// 		{
// 			FD_SET(client_sockets[i], &write_sockets);
// 			FD_SET(client_sockets[i], &read_sockets);
// 		}

// 		int max_socket = server_socket;
// 		for (long unsigned int i = 0; i < client_sockets.size(); i++)
// 		{
// 			max_socket = std::max(max_socket, client_sockets[i]);
// 		}

// 		int	activity = select(max_socket + 1, &read_sockets, &write_sockets, &error_sockets, NULL);
// 		if (activity < 0)
// 		{
// 			std::cerr << "Select error" << std::endl;
// 			return 1;
// 		}

// 		//New connection
// 		if (FD_ISSET(server_socket, &read_sockets))
// 		{
// 			int new_socket;

// 			if ((new_socket = accept(server_socket, (struct sockaddr *)NULL, NULL)) < 0)
// 			{
// 				std::cerr << "Accept failed" << std::endl;
// 				return 1;
// 			}
// 			std::cout << "New connection, socket fd is " << new_socket << std::endl;
//             client_sockets.push_back(new_socket);
// 		}
// 		else
// 		{
// 			//Read from client sockets
// 			std::vector<int> toDelete = client_sockets;
// 			for (long unsigned int i = 0; i < client_sockets.size(); i++)
// 			{
// 				int client_socket = client_sockets[i];

// 				if (FD_ISSET(client_socket, &read_sockets))
// 				{
// 					std::string httpRequest;

// 					while (true)
// 					{
// 						char buffer[BUFFER_SIZE];
// 						ssize_t bytesRead = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
// 						if (bytesRead == -1)
// 						{
// 							perror("recv");
// 							close(client_socket);
// 							toDelete.erase(toDelete.begin() + i);
// 							break;
// 						}
// 						else if (bytesRead == 0 && httpRequest.empty())
// 						{
// 							std::cout << "bytesRead == 0, closing connection" << std::endl;
// 							close(client_socket);
// 							toDelete.erase(toDelete.begin() + i);
// 							break;
// 						}
// 						else if (bytesRead < BUFFER_SIZE - 1) 
// 						{
// 							buffer[bytesRead] = '\0';
// 							httpRequest += buffer;
// 							std::cout << "Received complete HTTP Request:\n" << httpRequest << std::endl;
// 							break;
// 						}
// 						else 
// 						{
// 							buffer[bytesRead] = '\0';
// 							httpRequest += buffer;
// 						}
// 					}
// 				}

// 				//Write to client sockets
// 				else if (FD_ISSET(client_socket, &write_sockets))
// 				{
// 					//std::cout << "Write to client socket" << std::endl;
// 					const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!";
// 					ssize_t bytesSent = send(client_socket, response, strlen(response), 0);
// 					if (bytesSent == -1)
// 					{
// 						perror("send");
// 						close(client_socket);
// 						toDelete.erase(toDelete.begin() + i);
// 					}
// 					else if (bytesSent == 0)
// 					{
// 						close(client_socket);
// 						toDelete.erase(toDelete.begin() + i);
// 					}
// 					else 
// 					{
// 						std::cout << "Data sent" << std::endl;
// 						close(client_socket);
// 						toDelete.erase(toDelete.begin() + i);
// 						std::cout << "Connection closed" << std::endl;
// 					}
// 				}

// 				//Error on client sockets
// 				else if (FD_ISSET(client_socket, &error_sockets))
// 				{
// 					std::cerr << "Error on client socket" << std::endl;
// 					close(client_socket);
// 					toDelete.erase(toDelete.begin() + i);
// 				}
// 			}
// 			client_sockets = toDelete;
// 		}
// 	}
// 	return 0;
// }