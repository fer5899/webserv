#include "../include/Response.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Location.hpp"
#include "../include/Request.hpp"

void	test_error_response_from_default_error_pages()
{

}

void	test_error_response_with_no_default_error_page()
{

}

void	test_error_response_error_code_from_request()
{

}

void	test_error_response_location_not_found()
{

}

void	test_error_response_method_not_allowed()
{

}

void	test_redirection()
{

}

void	test_get_file()
{

}

void	test_get_file_not_found()
{

}

void	test_get_directory_with_index()
{

}

void	test_get_directory_no_index_with_autoindex()
{

}

void	test_get_directory_no_index_no_autoindex()
{

}

void	test_file_upload_no_upload_store()
{

}

void	test_file_upload_no_content_type_in_request()
{

}

void	test_file_upload_wrong_content_type_in_request()
{

}

void	test_file_upload()
{

}

void	test_delete_file()
{

}

void	test_delete_file_not_found()
{

}

std::string generateTimestamp() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
	std::string	separator = "_";

    std::stringstream timestamp;
    timestamp << localTime->tm_year + 1900 << separator
              << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1 << separator
              << std::setw(2) << std::setfill('0') << localTime->tm_mday << separator
              << std::setw(2) << std::setfill('0') << localTime->tm_hour << separator
              << std::setw(2) << std::setfill('0') << localTime->tm_min << separator
              << std::setw(2) << std::setfill('0') << localTime->tm_sec;

    return timestamp.str();
}

// TEST RESPONSE
int	main(void)
{

	std::cout << generateTimestamp() << std::endl;

}