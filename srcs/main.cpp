#include "../include/Server.hpp"
#include "../include/ConnectionManager.hpp"
#include "../include/Configuration.hpp"
#include "../include/common.hpp"

void	sigintHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << std::endl << RED "Webserv stopped" RESET << std::endl;
		exit(0);
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << RED "Usage: ./webserv [config_file]" RESET << std::endl;
		return 1;
	}
	signal(SIGINT, sigintHandler);
	std::string filename(argv[1]);
	Configuration config(filename);
	config.parseConfigFile(filename);

	config.printConfig();

	ConnectionManager cm(config);

	cm.printServers();

	try
	{
		cm.setUpServers();
		cm.runServers();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}


// MAIN PARA PROBAR EL PARSER DE REQUESTS

// int main(int argc, char** argv)
// {
// 	std::cout << GREEN "Usage: ./server [get|post|delete]" << std::endl << "Default: post" RESET << std::endl;
// 	const char* filename = "http_raw/post.txt";
// 	if (argc == 2)
// 	{
// 		if (strcmp(argv[1], "get") == 0)
// 			filename = "http_raw/get.txt";
// 		else if (strcmp(argv[1], "post") == 0)
// 			filename = "http_raw/post.txt";
// 		else if (strcmp(argv[1], "delete") == 0)
// 			filename = "http_raw/delete.txt";
// 		else
// 			return (std::cerr << RED "Invalid argument" RESET << std::endl, 1);
// 	}

// 	FILE* archivo = fopen(filename, "rb");
// 	if (archivo == NULL)
// 	{
// 		std::cerr << RED "Error: Can't open the file" RESET<< std::endl;
// 		return 1;
// 	}

// 	Request Request;
// 	const int buffer_size = 8; // Tamaño del buffer en bytes
// 	char buffer[buffer_size];
// 	std::string buffer_str;
// 	std::string content;
// 	while (fgets(buffer, buffer_size, archivo) != NULL)
// 	{
// 		buffer_str = buffer;
// 		content+= buffer_str;
// 		if (Request.parseRequest(buffer_str))
// 			break;
// 	}
// 	fclose(archivo);

// 	std::cout << std::endl << YELLOW "HTTP request raw content:" RESET << std::endl << content << std::endl << std::endl;

// 	std::cout << YELLOW "HTTP request parsed:" RESET << std::endl << Request << std::endl;
// 	return 0;
// }