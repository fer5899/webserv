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

int countNewlines(const std::string& text)
{
    int counter = 0;
    for (size_t i = 0; i < text.length(); ++i) {
        if (text[i] == '\n') {
			if (i > 0 && text[i - 1] != '\r')
            	counter++;
        }
    }
    return counter;
}