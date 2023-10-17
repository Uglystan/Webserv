#include "../../include/serveur.hpp"

/*Premiere fonction on accepte une nouvelle connexion la nombre de connexion est limitee a 250 pour eviter les probleme de limite fd
on initialise aussi le timer de timeout de la socket et on l'ajout a la gestion d'evenement*/

/*Deuxieme fonction : a chaque fois au'un socket est activee en lecture on vient lire de maniere non bloquante les donne et on les stocke dans la map
qui aussi le num de socket avec le message et le nombre de caractere lu ensuite on remet le temps de time out de la socket a 0 car on a eu une
activite dessus et on verifie si le message est complet dans ce cas on va le traiter sinon on continue la surveillance des socket jusqu'a avoir une message complet
et si on a une erreur au moment du recv on suppr le client*/

void	acceptNewClient(t_server &data, int &serverSocket)
{
	int clientSocket;
	struct sockaddr_in clientAdresse;
	int addresseLen = sizeof(clientAdresse);
	struct timeval time;

	if (data.timer.size() <= 250)//Pas plus de 250 socket ouverte sinon limite de fs atteinte a 1024
	{
		clientSocket = accept(serverSocket, (struct sockaddr*) &clientAdresse, (socklen_t *) &addresseLen);
		if (clientSocket != -1)
		{
			//On met la socket en non bloquant c'est important sinon ca marche pas
			int opt = 1;
			if (setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(1)) == -1)
				throw errorStopServ(strerror(errno));
			gettimeofday(&time, NULL);//creation init
			data.timer[clientSocket] = time;
			addEpollEvent(data.epollFd, clientSocket);
		}
	}
}

void	manageClient(t_server &data, int &clientSocket)
{
	char	buffer[1024];
	size_t	sizeHeader = 0;
	int	b = 0;

	ft_memset(buffer, 0, sizeof(buffer));
	b = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);
	data.req[clientSocket].bytes += b;
	data.req[clientSocket].message.append(buffer, b);
	sizeHeader = data.req[clientSocket].message.find("\r\n\r\n");
	if (data.req[clientSocket].bytes == -1 || data.req[clientSocket].bytes == 0)
	{
		if (data.req[clientSocket].bytes == -1)
			throw errorContinueServ("500", findGoodServ(data.req[clientSocket].message, data, clientSocket), clientSocket);
		else
			std::cout << "Client deco" << std::endl;
		data.req.erase(clientSocket);
		data.timer.erase(clientSocket);
		delEpollEvent(data.epollFd, clientSocket);
		if (close(clientSocket) == -1)
			throw errorStopServ(strerror(errno));
	}
	else if (data.req[clientSocket].bytes > 0)
	{
		gettimeofday(&data.timer[clientSocket], NULL);//reset
		if (data.req[clientSocket].message.find("Content-Length: ") != std::string::npos)//Post en plusieur morceau
			recvMulti(data, clientSocket, sizeHeader);
		else if (data.req[clientSocket].message.find("Content-Length: ") == std::string::npos)
		{
			if (data.req[clientSocket].message.find("Transfer-Encoding") == std::string::npos && data.req[clientSocket].message.size() >= sizeHeader)//Get base
				recvBase(data, clientSocket);
			else if (data.req[clientSocket].message.find("Transfer-Encoding") != std::string::npos)//Requete chunk
				recvChunk(data, clientSocket);
		}
	}
}

void	disconnectClient(t_server &data, int &socket)
{
	//std::cout << "Deconnexion socket : " << socket << std::endl;
	data.timer.erase(socket);
	delEpollEvent(data.epollFd, socket);
	if (close(socket) == -1)
		throw errorStopServ(strerror(errno));
}
