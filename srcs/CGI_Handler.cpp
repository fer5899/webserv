#include "../include/CGI_Handler.hpp"

CGI_Handler::CGI_Handler(): _env(), _body(""){}

CGI_Handler::~CGI_Handler() {}

CGI_Handler::CGI_Handler(const CGI_Handler& other): _env(other._env), _body(other._body) {}

CGI_Handler& CGI_Handler::operator=(const CGI_Handler& other)
{
	if (this != &other)
	{
		_env = other._env;
		_body = other._body;
	}
	return *this;
}