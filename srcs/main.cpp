#include "../include/Server.hpp"
#include "../include/ConnectionManager.hpp"

int main()
{
	Server s1(4242);
	Server s2(4243);
	std::vector<Server> servers;
	servers.push_back(s1);
	servers.push_back(s2);
	ConnectionManager cm(servers);

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

// 	RequestHandler requestHandler;
// 	const int buffer_size = 8; // Tamaño del buffer en bytes
// 	char buffer[buffer_size];
// 	std::string buffer_str;
// 	std::string content;
// 	while (fgets(buffer, buffer_size, archivo) != NULL)
// 	{
// 		buffer_str = buffer;
// 		content+= buffer_str;
// 		if (requestHandler.parseRequest(buffer_str))
// 			break;
// 	}
// 	fclose(archivo);

// 	std::cout << std::endl << YELLOW "HTTP request raw content:" RESET << std::endl << content << std::endl << std::endl;

// 	std::cout << YELLOW "HTTP request parsed:" RESET << std::endl << requestHandler << std::endl;
// 	return 0;
// }
