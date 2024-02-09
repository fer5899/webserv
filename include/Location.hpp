#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>

class Location
{
	private:
		std::string				path;
		std::string				root;
		std::vector<int>		methods;
		bool					autoindex;
		std::string				index;
		std::string				redirection;
		std::string				uploads;
		std::string				cgi_path;
		std::string				cgi_ext;


	public:
		Location(/* args */);
		~Location();

		std::string			getPath();
		std::string			getRoot();
		std::vector<int>	getMethods();
		bool				getAutoindex();
		std::string			getIndex();
		std::string			getRedirection();
		std::string			getUploads();
		std::string			getCgiPath();
		std::string			getCgiExt();

};

#endif