#include "../../include/serveur.hpp"

int	searchPortRequest(std::string &message)
{
	int i = message.find("Host: ") + 6;

	if (message.find("Host: ") != std::string::npos)
	{
		while (message[i] != ':')
			i++;
		i++;
		return (atoi(message.substr(i, 6).c_str()));
	}
	return (8080);
}

std::string	searchNameRequest(std::string &message)
{
	int i = message.find("Host: ") + 6;
	int j = i;

	if (message.find("Host: ") != std::string::npos)
	{
		while (message[j] != ':')
			j++;
		return (message.substr(i, j - i).c_str());
	}
	return ("127.0.0.1");
}

t_configServ	findGoodServ(std::string &message, t_server &data, int clientSocket)
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
			{
				std::cout << data.config[i].serverName << std::endl;
				return (data.config[i]);
			}
		}
	}
	else if (nbrServMatch > 1)
	{
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port && data.config[i].serverName == name)
			{
				std::cout << data.config[i].serverName << std::endl;
				return (data.config[i]);
			}
		}
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port)
			{
				std::cout << data.config[i].serverName << std::endl;
				return (data.config[i]);
			}
		}
	}
	else
	{
		throw errorContinueServ("400", null, clientSocket);
	}
	return (null);
}