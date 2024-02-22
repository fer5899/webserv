#include <vector>
#include <string>
#include <iostream>

void	parseUploadBody(std::string body, std::string boundary, std::vector<std::string> &form_elements_filenames, std::vector<std::string> &form_elements_contents)
{
	std::string	section;
	std::string scd; // Section content disposition
	std::string filename;
	std::string s_body; // Section body
	size_t start_pos = 0;
	size_t end_pos = 0;

	while ((end_pos = body.find("--" + boundary, start_pos)) != std::string::npos)
	{
		section = body.substr(start_pos, end_pos - start_pos);
		scd = section.substr(0, section.find("\r\n", 2));
		size_t body_start = section.find("\r\n\r\n") + 4;
		size_t filename_pos = scd.find("filename=");
		if (filename_pos != std::string::npos)
		{
			if (section.size() > body_start + 2)
			{
				// Parse filename from Content-Disposition
				filename = scd.substr(filename_pos + 9, scd.find_first_of(";\r", filename_pos + 9) - filename_pos - 9);
				// Trim the filename of leading and trailing whitespaces and quotes
				size_t start = filename.find_first_not_of(" \"");
				size_t end = filename.find_last_not_of(" \"");
				filename = filename.substr(start, end - start + 1);
				// Get the body of the section
				s_body = section.substr(body_start, section.size() - body_start - 2);
				// Store the filename and body
				form_elements_filenames.push_back(filename);
				form_elements_contents.push_back(s_body);
			}
		}
		start_pos = end_pos + boundary.size() + 2;
	}
}

int	main(void)
{
	std::vector<std::string> form_elements_filenames;
	std::vector<std::string> form_elements_contents;

	// Parse boundary from Content-Type header
	std::string boundary = "AaB03x";
	std::string body = "--AaB03x\r\nContent-Disposition: form-data; name=\"files\"; filename=\" file2.txt\"; hola \r\nContent-Type: text/plain\r\n\r\n... contents of file2.txt ...\r\n--AaB03x\r\nContent-Disposition: form-data; name=\"submit-name\"\r\n\r\nfilename=\r\n--AaB03x\r\nContent-Disposition: form-data; name=\"files\"; filename=\" file1.txt\"\r\nContent-Type: text/plain\r\n\r\n... contents of file1.txt ...\r\n--AaB03x--\r\n";

	// Parse sections of the form from the body, only keep elements that have filename= in Content-Disposition
	parseUploadBody(body, boundary, form_elements_filenames, form_elements_contents);

	// Print the filenames and contents
	for (size_t i = 0; i < form_elements_filenames.size(); i++)
	{
		std::cout << "Filename: " << form_elements_filenames[i] << std::endl;
		std::cout << "Content: " << form_elements_contents[i] << std::endl;
	}
}