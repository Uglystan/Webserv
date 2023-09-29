#include "../../include/serveur.hpp"

int	getFile(char **argv, std::string &stringFile)
{
	std::ifstream	file(argv[1], std::ios::in);
	std::string	line;

	if (file.is_open() == false)
		return (-1);
	while (std::getline(file, line))
		stringFile.append(line);
	return (0);
}


int	checkEachServString(std::vector<std::string> eachServString)
{
	unsigned int	i = 0;

	while(i < eachServString.size())
	{
		if (eachServString[i].find("listen ") == std::string::npos || eachServString[i].find("listen ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("listen ") + 7] <= 32)
			return (-1);
		if (eachServString[i].find("root ") == std::string::npos || eachServString[i].find("root ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("root ") + 5] <= 32)
			return (-1);
		if (eachServString[i].find("index ") == std::string::npos || eachServString[i].find("index ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("index ") + 6] <= 32)
			return (-1);
		if (eachServString[i].find("error_page ") == std::string::npos || eachServString[i].find("error_page ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("error_page ") + 11] <= 32)
			return (-1);
		if (eachServString[i].find("max_body_size ") == std::string::npos || eachServString[i].find("max_body_size ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("max_body_size ") + 14] <= 32)
			return (-1);
		if (eachServString[i].find("cgi ") == std::string::npos || eachServString[i].find("cgi ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("cgi ") + 4] <= 32)
			return (-1);
		if (eachServString[i].find("allow_methods ") == std::string::npos || eachServString[i].find("allow_methods ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("allow_methods ") + 14] <= 32)
			return (-1);
		i++;
	}
	return (0);
}

/*Une socket par couple adresseIP/port different et si sur le meme adresseIP/port il y'a 2 serveur alors on regarde HOST*/
int	parsingConf(t_server &data, char **argv)
{
	std::string	stringFile;
	std::vector<std::string>	eachServString;
	(void)data;

	if (getFile(argv, stringFile) == -1)
	{
		std::cout << "Error getFile" << std::endl;
		return (-1);
	}
	if (cutEachServ(stringFile, eachServString) == -1)
	{
		std::cout << "Error cutEachServ" << std::endl;
		return (-1);
	}
	if (checkEachServString(eachServString) == -1)
	{
		std::cout << "CheckEachServString" << std::endl;
		return (-1);
	}
	if (fillServ(data.config, eachServString) == -1)
	{
		std::cout << "Error FillServ" << std::endl;
		return (-1);
	}
	if (checkData(data.config) == -1)
	{
		std::cout << "Error FillServ" << std::endl;
		return (-1);
	}
	return (0);
}