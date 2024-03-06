# ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <sys/select.h>
# include <cstring>
# include <string>
# include <unistd.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include "Location.hpp"

class Server
{
	public:
		Server();
		Server(int port);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		void						setUpServer();

		int							getSocket() const;
		std::string					getServerName() const;
		int							getPort() const;
		std::map<int, std::string>	getDefError() const;
		int							getMaxBodySize() const;
		std::vector<Location>		&getLocations();
		Location					&getLocation(int i);
		std::string					getRoot() const;
		std::string					getIndex() const;

		void						setErrorPage(int error_code, std::string error_page_path);
		void						setServerName(std::string server_name);
		void						setPort(int port);
		void						setDefError(std::map<int, std::string> error_page);
		void						setMaxBodySize(int max_body_size);
		void						setLocations(std::vector<Location> &locations);
		void						setRoot(std::string root);
		void						setIndex(std::string index);

		void						addLocation(Location &location);

	private:
		int							_port;
		int							_server_socket; // 0 - 65535
		struct sockaddr_in			_address;
		std::string					_server_name;
		std::map<int, std::string>	_error_page; // Each must be abspath, after finishing parse, each path will be location interpreted
		int							_max_body_size;
		std::vector<Location>		_locations;
		std::string					_root; // must be abspath
		std::string					_index; // cannot be a directory, cannot start with /
};

#endif
