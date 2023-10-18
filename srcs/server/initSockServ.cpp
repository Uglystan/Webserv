#include "../../include/serveur.hpp"

/*Ici on va "Demarrer" les serveurs donc creer une socket pour chaque IP/Port du file conf et si 2 serveurs on le meme ip et port il n'y aura
qu'une seule socket pour les 2 c'est apres qu'il faudra faire la distinction notamment avec le server name"*/

void	checkSameIp(t_server &data, t_configServ serv)
{
	unsigned int	i = 0;

	while (i < data.config.size())
	{
		if (data.config[i].serverSocket == -1)//si pas de socket serv
		{
			if (data.config[i].ip == serv.ip && data.config[i].port == serv.port)
			{
				data.config[i].serverSocket = serv.serverSocket;
			}
		}
		i++;
	}
}

int	initAllServ(t_server &data)
{
	unsigned int	i = 0;
	int	opt = 1;

	while (i < data.config.size())
	{
		if (data.config[i].serverSocket == -1)//si pas de socket serv
		{
			data.adresse.sin_family = AF_INET;
			data.adresse.sin_addr.s_addr = inet_addr(data.config[i].ip.c_str());
			data.adresse.sin_port = htons(data.config[i].port);

			if ((data.config[i].serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				std::cerr << "Echec de la creation socket" << std::endl;
				return (-1);
			}
			// Parametre de la socket on lui dis de pouvoir utiliser la meme adresse et d'ecouter sur le meme port donc on peut gerer plusieur requete
			if (setsockopt(data.config[i].serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
			{
				std::cerr << "Echec de la mise en place des options socket" << std::endl;
				return (-1);
			}
			//On place la socket sur le port et l'adresse IP defini
			if (bind(data.config[i].serverSocket, (struct sockaddr *) &data.adresse, sizeof(data.adresse)) != 0)
			{
				std::cerr << "Echec de bind" << std::endl;
				return (-1);
			}
			//On fait ecouter la socket pour recevoir des requetes 2eme opt taille de file d'attente
			if (listen(data.config[i].serverSocket, 10) != 0)
			{
				std::cerr << "Echec de listen" << std::endl;
				return (-1);
			}
			addEpollEvent(data.epollFd, data.config[i].serverSocket);
			checkSameIp(data, data.config[i]);
			std::cout << "Socket " << data.config[i].serverSocket << " pour serveur " << data.config[i].serverName << " en marche\n" << std::endl;
		}
		i++;
	}
	return (0);
}