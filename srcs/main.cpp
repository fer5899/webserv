#include <vector>
#include <string>
#include <iostream>

int	main(void)
{
	std::vector<std::string>		locations;
	locations.push_back("/docs/fusion/");
	locations.push_back("/hola/");
	locations.push_back("/");
	locations.push_back("/docs/");
	if (locations.empty())
		std::cout << "no locations found" << std::endl;
	size_t	max_loc_size = 0;
	std::string matched_loc;
	std::string	req_path = "/docs/fusion/index.html";
	std::string loc_path;

	for (size_t i = 0; i < locations.size(); i++)
	{
		loc_path = locations[i];
		if (loc_path.size() <= req_path.size())
		{
			if (req_path.compare(0, loc_path.size(), loc_path) == 0
				&& loc_path.size() > max_loc_size)
			{
				matched_loc = locations[i];
				max_loc_size = loc_path.size();
			}
		}
	}
	if (max_loc_size)
		std::cout << matched_loc << std::endl;
	else
		std::cout << "no locations found" << std::endl;
}