#include "../../include/serveur.hpp"

unsigned int	sizeLocation(std::string servString)
{
	unsigned int	i = servString.find("location ");

	while (servString[i] != '}' && i < servString.size())
		i++;
	return (i - servString.find("location "));
}

std::string	locationDirectory(std::string servString)
{
	return (servString.substr(servString.find("location ") + 9, (servString.find("{") - 1) - (servString.find("location ") + 9)));
}

void	getLocation(std::vector<t_location> &locationVec, std::string servString)
{
	t_location	location;

	if (servString.find("location ") == std::string::npos)
	{
		location.directory = "";
		location.allow_methods = ""; 
		location.root = "";
	}
	else
	{
		while (servString.find("location ") != std::string::npos)
		{
			location.directory = locationDirectory(servString.substr(servString.find("location "), sizeLocation(servString)));
			location.allow_methods = getStrInfo(servString.substr(servString.find("location "), sizeLocation(servString)), "allow_methods ");
			location.root = getStrInfo(servString.substr(servString.find("location "), sizeLocation(servString)), "root ");
			locationVec.push_back(location);
			servString.erase(servString.find("location "), sizeLocation(servString));
		}
	}
}