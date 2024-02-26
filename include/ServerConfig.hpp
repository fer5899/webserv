#pragma once

#include "common.hpp"
#include "LocationConfig.hpp"

class ServerConfig
{
	public:
		ServerConfig();
		~ServerConfig();
		ServerConfig(const ServerConfig &other);
		ServerConfig &operator=(const ServerConfig &other);

		void parseServerConfig(std::string line);
		void addLocation(LocationConfig location);
		std::vector<LocationConfig> &getLocations();
		void printServerConfig();
	
	private:
		std::map<std::string, std::string> _config;
		std::vector<std::string> _error_page;
		std::vector<LocationConfig> _locations;
};
