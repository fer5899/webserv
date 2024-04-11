#include "../include/Server.hpp"
#include "../include/ConnectionManager.hpp"
#include "../include/Configuration.hpp"
#include "../include/common.hpp"

ConnectionManager cm;

void	sigintHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << std::endl << RED "Webserv stopped" RESET << std::endl;
		for (int i = 0; i <= cm._max_socket; i++)
		{
			if (cm.getClientBySocket(i) != NULL)
			{
				cm.getClientBySocket(i)->clearRequest();
				cm.getClientBySocket(i)->clearResponse();
			}
		}
		exit(0);
	}
}
void printLogo()
{
	std::cout << "\n" << BLUE << std::endl;
	std::cout << "██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗" << std::endl;
	std::cout << "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║" << std::endl;
	std::cout << "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║" << std::endl;
	std::cout << "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝" << std::endl;
	std::cout << "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ " << std::endl;
	std::cout << " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  " << std::endl;
	std::cout << "\n" << CYAN << std::endl;
	std::cout << "---------------------  Created by -------------------------" << std::endl;
	std::cout << "------------  alvgomez && fgomez-d && javiersa ------------" << std::endl;
	std::cout << RESET << std::endl;
}

int main(int argc, char** argv)
{
	printLogo();
	if (argc != 2)
	{
		std::cerr << RED "Usage: ./webserv [config_file]" RESET << std::endl;
		return 1;
	}
	signal(SIGINT, sigintHandler);
	std::string filename(argv[1]);
	Configuration config(filename);
	config.parseConfigFile(filename);

	// config.printConfig();

	cm.buildServers(config.getServers());

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