#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>

class Location
{
	private:
		std::string				_path;
		std::string				_root;
		std::string				_alias;
		std::vector<int>		_methods;
		bool					_autoindex;
		std::string				_index;
		int						_redir_code;
		std::string				_redir_url;
		std::string				_uploads;
		std::string				_cgi_path;
		std::string				_cgi_ext;


	public:
		Location(/* args */);
		~Location();

		std::string			getFilePathPrefix();
		std::string			getPath();
		std::string			getRoot();
		std::string			getAlias();
		std::vector<int>	getMethods();
		bool				getAutoindex();
		std::string			getIndex();
		int					getRedirCode();
		std::string			getRedirUrl();
		std::string			getUploads();
		std::string			getCgiPath();
		std::string			getCgiExt();

};

#endif