#include "../../include/serveur.hpp"

int	checkBracket(std::string stringFile)
{
	unsigned long i = 0;
	int j = 0;
	while (i <= stringFile.size())
	{
		if (stringFile[i] == '{' || stringFile[i] == '}')
			j++;
		i++;
	}
	if (j == 0 || j % 2 != 0)
	{
		std::cout << "Error bracket number" << std::endl;
		return (-1);
	}
	return (0);
}

int	lenServ(std::string stringFile)
{
	int nbopen = 1;
	int nbclose = 0;
	unsigned long i = stringFile.find("{") + 1;

	if (i == std::string::npos || checkBracket(stringFile) == -1)
	{
		std::cout << "Error bracket number" << std::endl;
		return (-1);
	}
	while (nbopen != nbclose && i <= stringFile.size())
	{
		if (stringFile[i] == '{')
			nbopen++;
		if (stringFile[i] == '}')
			nbclose++;
		i++;
	}
	if (i > stringFile.size())
		return (-1);
	return (i);
}

int	startServer(std::string stringFile)
{
	unsigned long i = stringFile.find("server");
	if (i == std::string::npos)
	{
		std::cout << "Missing block server" << std::endl;
		return (-1);
	}
	return (i);
}

int	cutEachServ(std::string &stringFile, std::vector<std::string> &eachServString)
{
	while (stringFile.find("server") != std::string::npos)
	{
		int start = startServer(stringFile);
		int len = lenServ(stringFile);
		if (len == -1 || start == -1)
			return (-1) ;
		eachServString.push_back(stringFile.substr(start, len));
		stringFile.erase(start, len);
	}
	return (0);
}