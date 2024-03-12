#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <iostream>
# include <map>
# include <vector>
# include "LocationConfig.hpp"
# include "ServerConfig.hpp"

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
		std::vector<std::string >	_cgi_path; // Must exist and be executable
		std::vector<std::string >	_cgi_ext; // Must start with . and only contain alpha


	public:
		Location();
		Location(LocationConfig &config, ServerConfig &server_config);
		Location(const Location &other);
		Location &operator=(const Location &other);
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
		std::vector<std::string >	getCgiPath();
		std::vector<std::string >	getCgiExt();

		void						setPath(std::string path);
		void						setRoot(std::string root);
		void						setAlias(std::string alias);
		void						setMethods(std::vector<std::string> methods);
		void						addMethod(std::string method);
		void						setAutoindex(bool autoindex);
		void						setIndex(std::string index);
		void						setRedirCode(int redir_code);
		void						setRedirUrl(std::string redir_url);
		void						setUploadStore(std::string upload_store);
		void						setCgiPath(std::vector<std::string > cgi_path);
		void						setCgiExt(std::vector<std::string > cgi_ext);

};

#endif