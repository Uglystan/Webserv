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