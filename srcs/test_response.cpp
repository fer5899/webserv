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
	Client client = Client(&server, 0);


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
	Client client = Client(&server, 0);

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
	Client client = Client(&server, 0);

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
	Client client = Client(&server, 0);

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
	Client client = Client(&server, 0);

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
	// Create server
	Server server = Server();

	// Create location
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);
	location.setRedirCode(301);
	location.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/this");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_file()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/test.html");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_file_not_found()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/hola.html");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_directory_with_index()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);
	location.setIndex("index.html");

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_directory_no_index_with_autoindex()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);
	location.setAutoindex(true);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_directory_no_index_no_autoindex()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "GET";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/testing/");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_file_upload_no_upload_store()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "POST";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("POST");
	request.setPath("/testing/");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_file_upload_no_content_type_in_request()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "POST";
	location.addMethod(method);
	location.setUploadStore("testing/upload_store");

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("POST");
	request.setPath("/testing/");
	request.setBody("test");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_file_upload_wrong_content_type_in_request()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "POST";
	location.addMethod(method);
	location.setUploadStore("testing/upload_store");

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("POST");
	request.setPath("/testing/");
	request.setBody("test");
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "text/plain";
	request.setHeaders(headers);

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_file_upload()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "POST";
	location.addMethod(method);
	location.setUploadStore("testing/upload_store");

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("POST");
	request.setPath("/testing/");
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "multipart/form-data; boundary=---------------------------9051914041544843365972754266";
	request.setHeaders(headers);
	request.setBody("-----------------------------9051914041544843365972754266\r\nContent-Disposition: form-data; name=\"file\"; filename=\"test.html\"\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<title>Test</title>\r\n</head>\r\n<body>\r\n<h1>Test</h1>\r\n</body>\r\n</html>\r\n-----------------------------9051914041544843365972754266--\r\n");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_delete_file()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "DELETE";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("DELETE");
	request.setPath("/testing/test2.html");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_delete_file_not_found()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/testing");
	std::string method = "DELETE";
	location.addMethod(method);

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("DELETE");
	request.setPath("/testing/nonexistent.html");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

void	test_get_file_alias()
{
	// Create server
	Server server = Server();

	// Create locations
	Location location = Location();
	location.setPath("/alias");
	std::string method = "GET";
	location.addMethod(method);
	location.setAlias("/testing");

	Location loc2 = Location();
	loc2.setPath("/");
	loc2.addMethod(method);
	loc2.setRedirCode(301);
	loc2.setRedirUrl("https://www.google.com");

	// Add location to server
	server.addLocation(loc2);
	server.addLocation(location);

	// Create client
	Client client = Client(&server, 0);

	// Create request
	Request request = Request();
	request.setMethod("GET");
	request.setPath("/alias/test.html");

	// Add request to client
	client.setRequest(&request);

	// Create response
	Response response = Response(&client, &request);

	// Check response
	std::string response_str = response.getHttpResponse();
	std::cout << response_str << std::endl;
}

// TEST RESPONSE
// int	main(void)
// {
// 	std::cout << "TEST 1" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_error_response_from_default_error_pages();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 2" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_error_response_with_no_default_error_page();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 3" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_error_response_error_code_from_request();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 4" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_error_response_location_not_found();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 5" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_error_response_method_not_allowed();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 6" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_redirection();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 7" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_file();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 8" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_file_not_found();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 9" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_directory_with_index();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 10" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_directory_no_index_with_autoindex();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 11" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_directory_no_index_no_autoindex();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 12" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_file_upload_no_upload_store();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 13" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_file_upload_no_content_type_in_request();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 14" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_file_upload_wrong_content_type_in_request();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 15" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_file_upload();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 16" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_delete_file();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 17" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_delete_file_not_found();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	std::cout << "TEST 18" << std::endl;
// 	std::cout << "-------------------------" << std::endl;
// 	test_get_file_alias();
// 	std::cout << "-------------------------" << std::endl << std::endl;

// 	return (0);
// }

