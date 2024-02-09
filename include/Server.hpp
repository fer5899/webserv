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
		std::string					server_name;
		std::string					host;
		int							port;
		std::map<int, std::string>	def_error;
		int							max_body_size;
		std::vector<Location>		&locations;
		int							socket;
		std::string					root;
		std::string					uploads;
		std::string					index;


	public:
		Server(/* args */);
		~Server();

};

#endif