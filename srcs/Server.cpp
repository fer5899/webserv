#include "../include/Server.hpp"

Server::Server(int port) : _port(port) {}

Server::~Server() {}

void Server::setUpServer()
{
	if ((this->_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "Socket creation error" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port selected
	int opt = 1;
	setsockopt(this->_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		
	int flags = fcntl(this->_server_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);	// Add non-blocking flag and set close-on-exec flag, how the subject says
	if (flags == -1)
	{
		std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	// flags = fcntl(this->_server_socket, F_SETFD, FD_CLOEXEC);	// Set close-on-exec flag, how it should be
	// if (flags == -1)
	// {
	// 	std::cerr << "Fcntl error: " << strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_address.sin_port = htons(this->_port);

	if (bind(this->_server_socket, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0)
	{
		std::cerr << "Bind failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(this->_server_socket, 512) < 0)
	{
		std::cerr << "Listen failed: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

int Server::getSocket() const
{
	return _server_socket;
} 
#include "../include/Request.hpp"

#include <iostream>
#include <fstream>
#include <string>


#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// std::map<std::string, std::string> decodeUrlEncodedFormData(const std::string& encodedFormData)
// {
// 	std::map<std::string, std::string> decodedData;
// 	std::istringstream iss(encodedFormData);

// 	std::string pair;
// 	while (std::getline(iss, pair, '&'))
// 	{
// 		std::istringstream pairStream(pair);
// 		std::string key_value;
// 		while (std::getline(pairStream, key_value, '='))
// 		{
// 			std::string decodedKey = "";
// 			std::string decodedValue = "";
// 			 unsigned int decodeChar;
// 			 int len = key_value.length();
// 			 for (int i = 0; i < len; i++)
// 			{
// 				 if (key_value[i] == '%')
// 				{
// 					 sscanf(key_value.substr(i + 1, 2).c_str(), "%x", &decodeChar);
// 					 decodedKey += static_cast<char>(decodeChar);
// 					 i += 2;
// 				 }
// 				 else if (key_value[i] == '+')
// 				{
// 					 decodedKey += ' ';
// 				 }
// 				 else
// 				{
// 					 decodedKey += key_value[i];
// 				 }
// 			 }
// 			 std::getline(pairStream, key_value, '=');
// 			 len = key_value.length();
// 			 for (int i = 0; i < len; i++)
// 			{
// 				 if (key_value[i] == '%')
// 				{
// 					 sscanf(key_value.substr(i + 1, 2).c_str(), "%x", &decodeChar);
// 					 decodedValue += static_cast<char>(decodeChar);
// 					 i += 2;
// 				 }
// 				 else if (key_value[i] == '+')
// 				{
// 					 decodedValue += ' ';
// 				 }
// 				 else
// 				{
// 					 decodedValue += key_value[i];
// 				 }
// 			 }
// 			 decodedData[decodedKey] = decodedValue;
// 		 }
// 	 }

// 	 return decodedData;
// }

// int main()
// {
// 	 std::string encodedData = "q=hello+world&lang=en%20o%20es&foo=bar&foo=baz&foo=qux&name=John+Doe&age=25&email=john%40doe.com";
// 	 std::map<std::string, std::string> decodedData = decodeUrlEncodedFormData(encodedData);

// 	 // Imprimir los pares clave-valor decodificados
// 	for (std::map<std::string, std::string>::iterator it = decodedData.begin(); it != decodedData.end(); ++it)
// 	{
// 		td::cout << "Clave: " << it->first << ", Valor: " << it->second << std::endl;
// 	}

// 	 return 0;
// }


int main(int argc, char** argv)
{
	std::cout << GREEN "Usage: ./server [get|post|delete]" << std::endl << "Default: post" RESET << std::endl;
	const char* filename = "http_raw/post.txt";
	if (argc == 2)
	{
		if (strcmp(argv[1], "get") == 0)
			filename = "http_raw/get.txt";
		else if (strcmp(argv[1], "post") == 0)
			filename = "http_raw/post.txt";
		else if (strcmp(argv[1], "delete") == 0)
			filename = "http_raw/delete.txt";
		else
			return (std::cerr << RED "Invalid argument" RESET << std::endl, 1);
	}

	FILE* archivo = fopen(filename, "rb");
	if (archivo == NULL)
	{
		std::cerr << RED "Error: Can't open the file" RESET<< std::endl;
		return 1;
	}

	RequestHandler requestHandler;
	const int buffer_size = 8; // Tamaño del buffer en bytes
	char buffer[buffer_size];
	std::string buffer_str;
	std::string content;
	while (fgets(buffer, buffer_size, archivo) != NULL)
	{
		buffer_str = buffer;
		content+= buffer_str;
		if (requestHandler.parseRequest(buffer_str))
			break;
	}
	fclose(archivo);

	std::cout << std::endl << YELLOW "HTTP request raw content:" RESET << std::endl << content << std::endl << std::endl;

	std::cout << YELLOW "HTTP request parsed:" RESET << std::endl << requestHandler << std::endl;
	return 0;
}
