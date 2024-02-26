#include"../include/Configuration.hpp"

Configuration::Configuration(std::string filename)
{
	checkConfigFile(filename);
}

Configuration::Configuration(const Configuration &other)
{
	*this = other;
}

Configuration &Configuration::operator=(const Configuration &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
	}
	return *this;
}

Configuration::~Configuration()
{
}

int checkServerLine(std::string line);
int countWords(std::string line);

void Configuration::checkConfigFile(std::string filename)
{
	std::ifstream file(filename.c_str());
	// check if file is open
	if (!file.is_open())
	{
		std::cerr << RED "Error: Can't open the file" RESET << std::endl;
		exit(1);
	}
	bool server = false;
	bool in_server = false;
	bool in_location = false;
	std::string line;
	std::string last_line;
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		// remove comments
		if (line.find("#") != std::string::npos)
			line = line.substr(0, line.find("#"));
		// check if that there is nothing after the semicolon
		if (line.find(";") != std::string::npos)
		{
			std::string line2 = line.substr(line.find(";") + 1);
			line = line.substr(0, line.find(";"));
			std::istringstream iss(line2);
			std::string word;
			int count = 0;
			
			while (iss >> word)
				count++;
			if (count > 0)
			{
				std::cerr << RED "Error: Invalid line - semicolon" RESET << std::endl;
				exit(1);
			}
		}
		// check the server line
		if (line.find("server") != std::string::npos && countWords(line) == 1)
		{
			checkServerLine(line);
			if (in_server == true)
			{
				std::cerr << RED "Error: Invalid line - server 0" RESET << std::endl;
				exit(1);
			}
			server = true;
			in_server = true;
		}
		// check location line
		if (line.find("location") != std::string::npos)
		{
			if (in_server == false || in_location == true)
			{
				std::cerr << RED "Error: Invalid line - location 1" RESET << std::endl;
				exit(1);
			}
			in_location = true;
		}
		// check if the opening braces in the server block are correct
		if (last_line.find("server") != std::string::npos && countWords(last_line) == 1)
		{
			if (countWords(line) != 1)
			{
				std::cerr << RED "Error: Invalid line - server 3" RESET << std::endl;
				exit(1);
			}
			else 
			{
				std::istringstream iss(line);
				std::string word;
				iss >> word;
				if (word != "{")
				{
					std::cerr << RED "Error: Invalid line - server 4" RESET << std::endl;
					exit(1);
				}
			}
		}
		// check if the opening braces in the location block are correct
		else if (last_line.find("location") != std::string::npos && countWords(last_line) == 2)
		{
			if (countWords(line) != 1)
			{
				std::cerr << RED "Error: Invalid line - server 5" RESET << std::endl;
				exit(1);
			}
			else 
			{
				std::istringstream iss(line);
				std::string word;
				iss >> word;
				if (word != "{")
				{
					std::cerr << RED "Error: Invalid line - server 6" RESET << std::endl;
					exit(1);
				}
			}
		}
		// check if the braces in the location block are closed
		if (line.find("}") != std::string::npos && in_location == true)
		{
			if (in_server == false || countWords(line) != 1)
			{
				std::cerr << RED "Error: Invalid line - location 2" RESET << std::endl;
				exit(1);
			}
			else
			{
				std::istringstream iss(line);
				std::string word;
				iss >> word;
				if (word != "}")
				{
					std::cerr << RED "Error: Invalid line - location 3" RESET << std::endl;
					exit(1);
				}
			}
			in_location = false;
		}

		// check if the braces in the server block are closed
		else if (line.find("}") != std::string::npos && in_location == false)
		{
			if (in_server == false || countWords(line) != 1)
			{
				std::cerr << RED "Error: Invalid line - server 7" RESET << std::endl;
				exit(1);
			}
			else
			{
				std::istringstream iss(line);
				std::string word;
				iss >> word;
				if (word != "}")
				{
					std::cerr << RED "Error: Invalid line - server 8" RESET << std::endl;
					exit(1);
				}
			}
			in_server = false;
		}

		//check there are no openening braces lost
		if (line.find("{") != std::string::npos && last_line.find("server") == std::string::npos && last_line.find("location") == std::string::npos)
		{
			std::cerr << RED "Error: Invalid line - server 10" RESET << std::endl;
			exit(1);
		}
	last_line = line;
	}
	// check if there is a server
	if (server == false)
	{
		std::cerr << RED "Error: No server found" RESET << std::endl;
		exit(1);
	}
	//check if the server block is closed
	if (in_server == true)
	{
		std::cerr << RED "Error: Invalid line - server 9" RESET << std::endl;
		exit(1);
	}
	//check if the location block is closed
	if (in_location == true)
	{
		std::cerr << RED "Error: Invalid line - location 4" RESET << std::endl;
		exit(1);
	}
}

void Configuration::parseConfigFile(std::string filename)
{
	std::ifstream file(filename.c_str());
	std::string line;
	bool isServer = false;
	bool isLocation = false;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;
		// remove comments
		if (line.find("#") != std::string::npos)
			line = line.substr(0, line.find("#"));
		if (line.find("server") != std::string::npos && countWords(line) == 1)
		{
			isServer = true;
			while(isServer)
			{
				isServer = false;
				//std::cout << "------------ server found " << std::endl;
				ServerConfig server;
				_servers.push_back(server);
				std::getline(file, line);
				while(std::getline(file, line))
				{
					if (line.find("server") != std::string::npos && countWords(line) == 1)
					{
						isServer = true;
						break;
					}
					if (line.find("location") != std::string::npos)
					{
						isLocation = true;
						while(isLocation)
						{
							isLocation = false;
							//std::cout << "------------ location found " << std::endl;
							LocationConfig location;
							_servers.back().addLocation(location);
							std::getline(file, line);
							while(std::getline(file, line))
							{
								if (line.find("}") != std::string::npos)
								{
									isLocation = false;
									break;
								}
								if (line.empty())
									continue;
								if (line.find("#") != std::string::npos)
									line = line.substr(0, line.find("#"));
								if (line.find(";") != std::string::npos)
									line = line.substr(0, line.find(";"));
								_servers.back().getLocations().back().parseLocationConfig(line);
							}
						}
					}
					if (line.empty())
						continue;
					if (line.find("#") != std::string::npos)
						line = line.substr(0, line.find("#"));
					if (line.find(";") != std::string::npos)
						line = line.substr(0, line.find(";"));
					server.parseServerConfig(line);
				}
			}
		}
	}
}

int checkServerLine(std::string line)
{
	std::istringstream iss(line);
	std::string word;
	int count = 0;
	
	while (iss >> word)
	{
		if (word != "server")
		{
			std::cerr << RED "Error: Invalid line - server" RESET << std::endl;
			exit(1);
		}
		count++;
	}
	if (count > 1)
	{
		std::cerr << RED "Error: Invalid line - server" RESET << std::endl;
		exit(1);
	}
	return count;
}

int countWords(std::string line)
{
	std::istringstream iss(line);
	std::string word;
	int count = 0;
	
	while (iss >> word)
		count++;
	return count;
}

void Configuration::printConfig()
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Server " << i << std::endl;
		_servers[i].printServerConfig();
	}
}
