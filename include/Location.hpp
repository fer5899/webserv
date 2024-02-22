#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>

class Location
{
	private:
		std::string					_path; // Must be abspath
		std::string					_root; // Default: html, must be abspath, must be a directory
		std::string					_alias; // Must be abspath, must be a directory
		std::vector<std::string>	_methods; // "GET", "POST", "DELETE"
		bool						_autoindex;
		std::string					_index; // Default: index.html, cannot be a directory, cannot start with /
		int							_redir_code; // 300 301 302 303 304 307 308
		std::string					_redir_url;
		std::string					_upload_store; // Must be abspath, must be a directory, empty if not present
		std::string					_cgi_path; // Must exist and be executable
		std::string					_cgi_ext; // Must start with . and only contain alpha


	public:
		Location(/* args */);
		~Location();

		std::string					getPath();
		std::string					getRoot();
		std::string					getAlias();
		std::vector<std::string>	getMethods();
		bool						getAutoindex();
		std::string					getIndex();
		int							getRedirCode();
		std::string					getRedirUrl();
		std::string					getUploadStore();
		std::string					getCgiPath();
		std::string					getCgiExt();

};

#endif