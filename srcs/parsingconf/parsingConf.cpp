#include "../../include/serveur.hpp"

int	getFile(char **argv, std::string &stringFile, int argc)
{
	std::string	line;
	if (argc == 2)
	{
		std::ifstream	file(argv[1], std::ios::in);
		if (file.is_open() == false)
			return (-1);
		while (std::getline(file, line))
			stringFile.append(line);
	}
	else
	{
		std::ifstream	file("config/test.config", std::ios::in);
		if (file.is_open() == false)
			return (-1);
		while (std::getline(file, line))
			stringFile.append(line);
	}
	return (0);
}


int	checkEachServString(std::vector<std::string> eachServString)
{
	unsigned int	i = 0;

	while(i < eachServString.size())
	{
		if (eachServString[i].find("listen ") == std::string::npos || eachServString[i].find("listen ") > eachServString[i].find("location ") || eachServString[i][eachServString[i].find("listen ") + 7] <= 32)
			return (std::cout << "Missing listen line" << std::endl, -1);
		if (eachServString[i].find("root ") == std::string::npos || eachServString[i].find("root ") > eachServString[i].find("location "))
			return (std::cout << "Missing root" << std::endl, -1);
		if (eachServString[i].find("index ") == std::string::npos || eachServString[i].find("index ") > eachServString[i].find("location "))
			return (std::cout << "Missing index" << std::endl, -1);
		if (eachServString[i].find("error_page ") == std::string::npos || eachServString[i].find("error_page ") > eachServString[i].find("location "))
			return (std::cout << "Missing error_page" << std::endl, -1);
		if (eachServString[i].find("max_body_size ") == std::string::npos || eachServString[i].find("max_body_size ") > eachServString[i].find("location "))
			return (std::cout << "Missing max_body_size" << std::endl, -1);
		if (eachServString[i].find("cgi ") == std::string::npos || eachServString[i].find("cgi ") > eachServString[i].find("location "))
			return (std::cout << "Missing cgi" << std::endl, -1);
		if (eachServString[i].find("cgi_ext ") == std::string::npos || eachServString[i].find("cgi_ext") > eachServString[i].find("location "))
			return (std::cout << "Missing cgi_ext" << std::endl, -1);
		if (eachServString[i].find("allow_methods ") == std::string::npos || eachServString[i].find("allow_methods ") > eachServString[i].find("location "))
			return (std::cout << "Missing allow_methods" << std::endl, -1);
		if (eachServString[i].find("listening_file ") == std::string::npos || eachServString[i].find("listening_file ") > eachServString[i].find("location "))
			return (std::cout << "Missing listening_file" << std::endl, -1);
		i++;
	}
	return (0);
}

/*Une socket par couple adresseIP/port different et si sur le meme adresseIP/port il y'a 2 serveur alors on regarde HOST*/
int	parsingConf(t_server &data, char **argv, int argc)
{
	std::string	stringFile;
	std::vector<std::string>	eachServString;
	(void)data;

	if (getFile(argv, stringFile, argc) == -1)
	{
		std::cout << "Cannot open conf file" << std::endl;
		return (-1);
	}
	if (cutEachServ(stringFile, eachServString) == -1)
		return (-1);
	if (checkEachServString(eachServString) == -1)
		return (-1);
	if (fillServ(data.config, eachServString) == -1)
		return (-1);
	if (checkData(data.config) == -1)
		return (-1);
	return (0);
}

/*Getfile : Si on ne peut pas ouvrir le fichier de conf error*/
/*Cuteachserv : Si il manque des '{}' error. Si pas ecrit de server error*/
/*checkEachServString : Si ailleur que dans les blocs location pas de listen, root, index, error_page, max_body_size, cgi, allow_methods error*/
/*CheckData : Si IP format invalide ou IP invalide error, Si body_size_invalide error, SI port invalide error, */