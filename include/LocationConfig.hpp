#pragma once

# include "common.hpp"
# include "utils.hpp"
# include <unistd.h>

class LocationConfig
{
	public:
		LocationConfig();
		~LocationConfig();
		LocationConfig(const LocationConfig &other);
		LocationConfig &operator=(const LocationConfig &other);

		void parseLocationConfig(std::string line);
		void printLocationConfig();
		void setPath(std::string line);

		std::string getPath() const;
		std::string getRoot() const;
		std::string getAlias() const;
		std::vector<std::string> getMethods() const;
		bool getAutoindex() const;
		std::string getIndex() const;
		int getRedirCode() const;
		std::string getRedirUrl() const;
		std::string getUploadStore() const;
		std::map<std::string, std::string> getCgi() const;

	
	private:
		std::map<std::string, std::string> _locConfig;
		std::string _path;
		std::vector<std::string> _methods;
		std::vector<std::string> _cgiPath;
		std::vector<std::string> _cgiExt;
		std::vector<std::string> _return;
};