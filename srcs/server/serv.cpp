#include "../../include/serveur.hpp"

int main (/*File conf*/)
{
	int epollFd = epoll_create1(0);
	struct epoll_event	events[NB_EVENT];
	struct sockaddr_in	adresse;
	std::map <int, struct timeval>	timer;

	memset(events, 0, sizeof(events));
	initAdresse(adresse);
	int serverSocket = initSocket(adresse, epollFd);
	while(1)
	{
		//Le nombre de client qu'on souhaite gerer j'ai mis 2 cela depend du trafic que notre reseau va gerer si beaucoup de trafic
		// il vaut mieux un nombre eleve si probleme on monte le nombre inon peut etre file d'attente. nfds c'est le nombre d'evenement qui on un truc EPOLLIN etc...
		//Peut etre ajouter un timer pour chaque socket pour les closes si pas d'activite recu
		int nfds = checkTimeAndWaitPoll(epollFd, events, timer);
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == serverSocket)//C'est la socket serveur donc on recoit un nouveau client
				acceptNewClient(serverSocket, timer, epollFd);
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