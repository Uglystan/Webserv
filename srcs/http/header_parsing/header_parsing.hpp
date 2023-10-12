# ifndef HEADER_PARSING_HPP
#define HEADER_PARSING_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <sstream>
#include <cstdlib>

std::string	find_server(std::string servname);
std::string	find_date(void);
std::string	find_content_type(std::string &request, std::string &method);
std::string	find_langage(std::string &request);
std::string	find_content_lenght(std::string &body);
std::string	find_connection(void);
std::string	find_content_lang(std::string &request);
std::string	find_LastModified(std::string &path);
std::string	find_tranfertencoding(void);
std::string	find_WwwAuthenticate(int code);
std::string find_path(std::string &request, std::string &root);
int	check_req_code(std::string request);

#endif