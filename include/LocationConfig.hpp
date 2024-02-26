#pragma once

#include "common.hpp"

class LocationConfig
{
	public:
		LocationConfig();
		~LocationConfig();
		LocationConfig(const LocationConfig &other);
		LocationConfig &operator=(const LocationConfig &other);

		void parseLocationConfig(std::string line);
		void printLocationConfig();
	
	private:
		std::map<std::string, std::string> _locConfig;
		std::vector<std::string> _methods;
		std::vector<std::string> _cgiPath;
		std::vector<std::string> _cgiExt;
};