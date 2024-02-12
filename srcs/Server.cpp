#include "../include/Server.hpp"
#include "../include/RequestHandler.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    const char* filename = "srcs/post.txt";
    FILE* archivo = fopen(filename, "rb");

    if (archivo == NULL) {
        std::cerr << RED "Error al abrir el archivo" RESET<< std::endl;
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
		{
			break;
		}
	}
	fclose(archivo);

	std::cout << YELLOW "HTTP request content:" RESET<< std::endl;
	std::cout << content << std::endl;
	std::cout << std::endl;

	std::cout << YELLOW "HTTP request parsed: (PURA MAGIA)" RESET<<std::endl;
	if (requestHandler.getErrorCode() != 0)
	{
		std::cout << RED "Error code: " << requestHandler.getErrorCode() << RESET << std::endl;
		return 1;
	}
	std::cout << BLUE "Method: " RESET<< requestHandler.getMethod() << std::endl;
	std::cout << BLUE "URL: "RESET << requestHandler.getURL() << std::endl;
	std::cout << BLUE "HTTP version: "RESET << requestHandler.getHTTPVersion() << std::endl;
	std::cout << BLUE "\nHeaders:"RESET << std::endl;
	std::map<std::string, std::string> headers = requestHandler.getHeaders();
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
	std::cout << std::endl << BLUE "Body: " RESET << std::endl << requestHandler.getBody() << std::endl;

    return 0;
}
