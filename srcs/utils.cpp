#include "../include/utils.hpp"

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int countSubstring(const std::string& str, const std::string& sub)
{
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = str.find(sub); offset != std::string::npos; 
        offset = str.find(sub, offset + sub.length())) {
        ++count;
    }
    return count;
}

bool	isAbsPath(std::string str)
{
	// Check if the string starts with a '/' and does not contain two consecutive '/'
	if (str[0] == '/' && countSubstring(str, "//") == 0)
		return true;
	return false;
}

