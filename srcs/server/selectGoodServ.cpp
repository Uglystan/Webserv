#include "../../include/serveur.hpp"

int	searchPortRequest(std::string &message)
{
	int i = message.find("Host: ") + 6;

	while (message[i] != ':')
		i++;
	i++;
	return (atoi(message.substr(i, 6).c_str()));
}

std::string	searchNameRequest(std::string &message)
{
	int i = message.find("Host: ") + 6;
	int j = i;

	while (message[j] != ':')
		j++;
	return (message.substr(i, j - i).c_str());
}

t_configServ	findGoodServ(std::string &message, t_server &data)
{
	int	port = searchPortRequest(message);
	std::string	name = searchNameRequest(message);
	int	nbrServMatch = 0;
	t_configServ null; {};

	for(unsigned int i = 0; i < data.config.size(); i++)
	{
		if (data.config[i].port == port)
			nbrServMatch++;
	}
	if (nbrServMatch == 1)
	{
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port)
				return (data.config[i]);
		}
	}
	else if (nbrServMatch > 1)
	{
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port && data.config[i].serverName == name)
				return (data.config[i]);
		}
	}
	else
	{
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port)
				return (data.config[i]);
		}
	}
	return(null);
}