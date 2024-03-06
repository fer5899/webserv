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

		int getPort() const;
		std::string getServerName() const;
		std::string getRoot() const;
		std::string getIndex() const;
		std::map<int, std::string> getErrorPage() const;
		int getMaxBodySize() const;

		std::vector<LocationConfig> &getLocations();
	
	private:
		std::map<std::string, std::string> _config;
		std::vector<std::string> _error_page;
		std::vector<LocationConfig> _locations;
};
