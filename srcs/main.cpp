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