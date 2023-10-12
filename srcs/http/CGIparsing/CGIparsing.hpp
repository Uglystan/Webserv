# ifndef CGIPARSING_HPP
#define CGIPARSING_HPP

#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>

std::string extractContentType(std::string &request);
std::string extractContentLength(std::string &request, std::string postData);
std::string extractPostData(std::string &request, std::string &type);
std::string extractQueryString(std::string &request);
std::string find_cgi_path(std::string path, std::string ext);
int	delete_file(std::string &path);

#endif