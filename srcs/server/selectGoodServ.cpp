#include "../../include/serveur.hpp"

/*Ici une fois qu'on q recu le message avant d'appeler le CGI on va identifier sur/pour quel serveur est la requete
on distingue 3 cas. Le 1er : Il n'y a qu'un seul serveur qui correspond avec le port donc on sait que c'est celui la.
Le 2eme : Plusieur serveur sont sur le meme port donc on fait la distinction avec le server name.
Le 3eme : Plusieur serveur sont sur le meme port mais le serveur name ne permet pas d'identifier a qui il appartient donc par default on prend le premier qui match
par rapport au conf file*/

/*Par default si on ne trouve pas de port on mettra 8080*/

int	searchPortRequest(std::string &message)
{
	int i = message.find("Host: ") + 6;

	if (message.find("Host: ") != std::string::npos)
	{
		while (message[i] != ':' && message[i] != '\n')
			i++;
		if (message[i] == '\n')
			return (8080);
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
		while (message[j] != ':' && message[j] != '\n')
			j++;
		if (message[j] == '\n')
			return ("127.0.0.1");
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
				//std::cout << data.config[i].serverName << std::endl;
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
				//std::cout << data.config[i].serverName << std::endl;
				return (data.config[i]);
			}
		}
		for (unsigned int i = 0; i < data.config.size(); i++)
		{
			if (data.config[i].port == port)
			{
				//std::cout << data.config[i].serverName << std::endl;
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