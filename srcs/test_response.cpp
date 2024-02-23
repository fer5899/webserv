#include "../include/Response.hpp"
#include "../include/Client.hpp"
#include "../include/Server.hpp"
#include "../include/Location.hpp"
#include "../include/Request.hpp"

void	test_error_response_from_default_error_pages()
{
	// Create server
	Server server = Server();
	server.setErrorPage(500, "testing/error_pages/500.html");

	// Create client
	Client client(0, server);


	// Create request
	Request request = Request();
	request.setErrorCode(500);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
	
}

void	test_error_response_with_no_default_error_page()
{
	// Create server
	Server server = Server();

	// Create client
	Client client(0, server);

	// Create request
	Request request = Request();
	request.setErrorCode(500);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_error_response_error_code_from_request()
{
	// Create server
	Server server = Server();

	// Create client
	Client client(0, server);

	// Create request
	Request request = Request();
	request.setErrorCode(413);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_error_response_location_not_found()
{
	// Create server
	Server server = Server();

	// Create client
	Client client(0, server);

	// Create request
	Request request = Request();
	request.setPath("/testing/this/does/not/exist");

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_error_response_method_not_allowed()
{
	// Create server
	Server server = Server();

	// Create location
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);

	// Add location to server
	server.addLocation(location);

	// Create client
	Client client = Client(0, server);

	// Create request
	Request request = Request();
	request.setMethod("POST");
	request.setPath("/testing/this");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
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



// TEST RESPONSE
int	main(void)
{
	// std::cout << "TEST 1" << std::endl;
	// std::cout << "-------------------------" << std::endl;
	// test_error_response_from_default_error_pages();
	// std::cout << "-------------------------" << std::endl << std::endl;

	// std::cout << "TEST 2" << std::endl;
	// std::cout << "-------------------------" << std::endl;
	// test_error_response_with_no_default_error_page();
	// std::cout << "-------------------------" << std::endl << std::endl;

	// std::cout << "TEST 3" << std::endl;
	// std::cout << "-------------------------" << std::endl;
	// test_error_response_error_code_from_request();
	// std::cout << "-------------------------" << std::endl << std::endl;

	// std::cout << "TEST 4" << std::endl;
	// std::cout << "-------------------------" << std::endl;
	// test_error_response_location_not_found();
	// std::cout << "-------------------------" << std::endl << std::endl;

	std::cout << "TEST 5" << std::endl;
	std::cout << "-------------------------" << std::endl;
	test_error_response_method_not_allowed();
	std::cout << "-------------------------" << std::endl << std::endl;

	return (0);
}