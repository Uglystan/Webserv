# ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include "../utils/gnl.hpp"

std::string	execCgi(std::string path, std::string	_requestbody, std::string size, std::string &cgi_path)

#endif