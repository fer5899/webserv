#include "../include/Server.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::cout << "Hello, world!" << std::endl;
	std::ifstream archivo("srcs/get.txt");
	if (!archivo.is_open())
	{
		std::cerr << "Error al abrir el archivo" << std::endl;
		return 1;
	}
	std::string contenido((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
	std::cout << "HTTP request content:" << std::endl;
	std::cout << contenido << std::endl;
	archivo.close();

    return 0;
}
