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
		std::vector<Location>		_locations;
		int							_socket;
		std::string					_root; // must be abspath
		std::string					_index; // cannot be a directory, cannot start with /


	public:
		Server();
		Server(std::string server_name, std::string host, int port, std::map<int, std::string> error_page, int max_body_size, std::vector<Location> &locations, int socket, std::string root, std::string index);
		~Server();

		std::string					getServerName();
		std::string					getHost();
		int							getPort();
		std::map<int, std::string>	getDefError();
		int							getMaxBodySize();
		std::vector<Location>		getLocations();
		Location					&getLocation(int i);
		int							getSocket();
		std::string					getRoot();
		std::string					getIndex();

		void						setErrorPage(int error_code, std::string error_page_path);
		void						setServerName(std::string server_name);
		void						setHost(std::string host);
		void						setPort(int port);
		void						setDefError(std::map<int, std::string> error_page);
		void						setMaxBodySize(int max_body_size);
		void						setLocations(std::vector<Location> &locations);
		void						setSocket(int socket);
		void						setRoot(std::string root);
		void						setIndex(std::string index);

		void						addLocation(Location &location);

};

#endif