#include "../../include/serveur.hpp"

void	initAdresse(t_server &data)
{
	data.adresse.sin_family = AF_INET;
	data.adresse.sin_addr.s_addr = inet_addr("127.0.0.1");
	data.adresse.sin_port = htons(8080);
}

int	initSocket(t_server &data)
{
	int	socketServeur;
	int opt = 1;
	//Creation socket client
	if ((socketServeur = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cout << "Echec de la creation socket" << std::endl;
		/*Gestion erreur*/
	}
	// Parametre de la socket on lui dis de pouvoir utiliser la meme adresse et d'ecouter sur le meme port donc on peut gerer plusieur requete
	if (setsockopt(socketServeur, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
	{
		std::cout << "Echec de la mise en place des options socket" << std::endl;
		/*Gestion erreur*/
	}
	//On place la socket sur le port et l'adresse IP defini
	if (bind(socketServeur, (struct sockaddr *) &data.adresse, sizeof(data.adresse)) != 0)
	{
		perror("Erreur bind");
		std::cout << "Echec de bind" << std::endl;
		/*Gestion erreur*/
	}
	//On fait ecouter la socket pour recevoir des requetes 2eme opt taille de file d'attente
	if (listen(socketServeur, 10) != 0)
	{
		std::cout << "Echec de listen" << std::endl;
		/*Gestion erreur*/
	}
	addEpollEvent(data.epollFd, socketServeur);
	return (socketServeur);
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
				std::cout << "Echec de la creation socket" << std::endl;
				/*Gestion erreur*/
			}
			// Parametre de la socket on lui dis de pouvoir utiliser la meme adresse et d'ecouter sur le meme port donc on peut gerer plusieur requete
			if (setsockopt(data.config[i].serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
			{
				std::cout << "Echec de la mise en place des options socket" << std::endl;
				/*Gestion erreur*/
			}
			//On place la socket sur le port et l'adresse IP defini
			if (bind(data.config[i].serverSocket, (struct sockaddr *) &data.adresse, sizeof(data.adresse)) != 0)
			{
				perror("Erreur bind");
				std::cout << "Echec de bind" << std::endl;
				/*Gestion erreur*/
			}
			//On fait ecouter la socket pour recevoir des requetes 2eme opt taille de file d'attente
			if (listen(data.config[i].serverSocket, 10) != 0)
			{
				std::cout << "Echec de listen" << std::endl;
				/*Gestion erreur*/
			}
			addEpollEvent(data.epollFd, data.config[i].serverSocket);
		}
		//fonction si port et adresse sont pareil que celui qu'on vient de faire on met meme socket
		i++;
	}
	return (0);
}