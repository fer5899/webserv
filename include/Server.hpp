#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "Location.hpp"

class Server
{
	private:
		std::string					_server_name;
		std::string					_host;
		int							_port; // 0 - 65535
		std::map<int, std::string>	_error_page; // Each must be abspath, after finishing parse, each path will be location interpreted
		int							_max_body_size;
		std::vector<Location>		&_locations;
		int							_socket;
		std::string					_root; // must be abspath
		std::string					_index; // cannot be a directory, cannot start with /


	public:
		Server(/* args */);
		~Server();

		std::string					getServerName();
		std::string					getHost();
		int							getPort();
		std::map<int, std::string>	getDefError();
		int							getMaxBodySize();
		std::vector<Location>		getLocations();
		int							getSocket();
		std::string					getRoot();
		std::string					getUploads();
		std::string					getIndex();

};

#endif