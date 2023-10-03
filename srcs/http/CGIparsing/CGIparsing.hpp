# ifndef CGIPARSING_HPP
#define CGIPARSING_HPP

#include <iostream>
#include <sstream>


std::string extractContentType(std::string &request);
std::string extractContentLength(std::string &request);
std::string extractPostData(std::string &request, std::string &type);
std::string extractQueryString(std::string &request);

#endif