#include <sys/socket.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <map>
#include "../response/response.hpp"
#include <sys/time.h>

#define NB_EVENT 10
#define MAX_CLIENT 100
#define MAX_CLIENT_BODYSIZE 20000
#define MAZ_TIME_BEFOR_CLOSE 

void	initAdresse(struct sockaddr_in	&adresse)
{
	adresse.sin_family = AF_INET;
	adresse.sin_addr.s_addr = inet_addr("127.0.0.1");
	adresse.sin_port = htons(8080);
}

int	initSocket(struct sockaddr_in	&adresse)
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
	if (bind(socketServeur, (struct sockaddr *) &adresse, sizeof(adresse)) != 0)
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
	return (socketServeur);
}

//Fonction pour ajouter des socket aux evenement de Epool
void	addEpollEvent(int &epollFd, int &socket)
{
	//Ici la structure sert juste setup les evenements pour la socket et a l'ajouter a la liste d'evenement
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	//On gere Le in et si la socket se ferme
	event.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, socket, &event);
}

int	waitForClient(int &serverSocket, std::map<int, struct timeval> &timer)
{
	int clientSocket;
	struct sockaddr_in clientAdresse;
	int addresseLen = sizeof(clientAdresse);
	struct timeval time;

	clientSocket = accept(serverSocket, (struct sockaddr*) &clientAdresse, (socklen_t *) &addresseLen);
	if (clientSocket != -1)
	{
		//On met la socket en non bloquant c'est important sinon ca marche pas
		int opt = 1;
		setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(1));
		gettimeofday(&time, NULL);//creation init
		timer[clientSocket] = time;
	}
	return (clientSocket);
}

void	delEpollEvent(int &epollFd, int &socket)
{
	struct epoll_event event;

	memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	// On surveille les message entrant, la fermeture de la socket
	event.events = EPOLLIN | EPOLLRDHUP;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, socket, &event);
}

void	manageClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer)
{
	char	buffer[1025];
	int	bytes_read = 0;
	std::string	msg;

	memset(buffer, 0, sizeof(buffer));
	bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);
	if (bytes_read == -1)
	{
		std::cout << "Error recv" << std::endl;
		timer.erase(clientSocket);
		delEpollEvent(epollFd, clientSocket);
		close(clientSocket);
	}
	else if (bytes_read > 0)
	{
		gettimeofday(&timer[clientSocket], NULL);//reset
		msg.append(buffer);
		if (bytes_read == 1024)
		{
			while (bytes_read == 1024)
			{
				memset(buffer, 0, sizeof(buffer));
				bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);//proteger recv encore ?
				msg.append(buffer);
				if (msg.size() > MAX_CLIENT_BODYSIZE)
				{
					/*Error page 413*/
				}
			}
		}
		std::cout << "Message recu : " << msg << std::endl;
		Response resp(msg);
		std::string rep = resp.create_response();
		std::cout << "Message envoyee : " << rep << std::endl;
		send(clientSocket, rep.c_str(), rep.size(), 0);
	}
}

void	disconnectClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Deconnexion client" << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}

void	errorClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Error sur socket" << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}

int	checkTimeAndWaitPoll(int &epollFd, struct epoll_event *events, std::map<int, struct timeval> &timer)
{
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	std::map<int, struct timeval>::iterator i = timer.begin();
	std::map<int, struct timeval>::iterator temp = i;
	unsigned long e = 0;

	while (i != timer.end())
	{
		temp = i;
		i++;
		std::cout << (timeNow.tv_sec - temp->second.tv_sec) + (timeNow.tv_usec - temp->second.tv_usec) / 1000000 << std::endl;
		if ((timeNow.tv_sec - temp->second.tv_sec) + (timeNow.tv_usec - temp->second.tv_usec) / 1000000 > 10)
		{
			std::cout << "Delais trop long deco d'une socket" << std::endl;
			delEpollEvent(epollFd, const_cast<int &>(temp->first));//aaaaa
			close(temp->first);
			timer.erase(temp->first);
		}
		e++;
	}
	return (epoll_wait(epollFd, events, 5, -1));
}

int main (/*File conf*/)
{
	int epollFd = epoll_create1(0);
	struct epoll_event	events[NB_EVENT];
	struct sockaddr_in	adresse;
	std::map <int, struct timeval>	timer;

	memset(events, 0, sizeof(events));
	initAdresse(adresse);
	int serverSocket = initSocket(adresse);
	addEpollEvent(epollFd, serverSocket);
	int clientSocket;
	while(1)
	{
		//Le nombre de client qu'on souhaite gerer j'ai mis 2 cela depend du trafic que notre reseau va gerer si beaucoup de trafic
		// il vaut mieux un nombre eleve si probleme on monte le nombre inon peut etre file d'attente. nfds c'est le nombre d'evenement qui on un truc EPOLLIN etc...
		//Peut etre ajouter un timer pour chaque socket pour les closes si pas d'activite recu
		int nfds = checkTimeAndWaitPoll(epollFd, events, timer);
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == serverSocket)//C'est la socket serveur donc on recoit un nouveau client
			{
				clientSocket = waitForClient(serverSocket, timer);
				if (clientSocket != -1)
					addEpollEvent(epollFd, clientSocket);
			}
			else if (events[i].events & EPOLLRDHUP)//Absolument devant EPOLLIN
				disconnectClient(epollFd, events[i].data.fd, timer);
			else if (events[i].events & EPOLLIN)
				manageClient(epollFd, events[i].data.fd, timer);
			else if (events[i].events & EPOLLERR)
				errorClient(epollFd, events[i].data.fd, timer);
		}
	}
	close(serverSocket);
}