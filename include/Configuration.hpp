#pragma once

#include "common.hpp"
#include "ServerConfig.hpp"
#include "LocationConfig.hpp"

class Configuration
{
	public:
		Configuration(std::string filename);
		~Configuration();
		Configuration(const Configuration &other);
		Configuration &operator=(const Configuration &other);

		void checkConfigFile(std::string filename);
		void parseConfigFile(std::string filename);
		void printConfig();

		std::vector<ServerConfig > &getServers();
	
	private:
		std::vector<ServerConfig > _servers;

};

