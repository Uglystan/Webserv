#include "httpparsing.hpp"

std::streampos GetFileSize(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file)
		return (-1);
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.close();
	return (fileSize);
}