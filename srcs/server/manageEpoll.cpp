#include "../../include/serveur.hpp"

int	checkTimeAndWaitPoll(int &epollFd, std::vector<struct epoll_event> &events, std::map<int, struct timeval> &timer)
{
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	std::map<int, struct timeval>::iterator i = timer.begin();
	std::map<int, struct timeval>::iterator temp = i;

	while (i != timer.end())
	{
		temp = i;
		i++;
		if ((timeNow.tv_sec - temp->second.tv_sec) + (timeNow.tv_usec - temp->second.tv_usec) / 1000000 > MAX_TIME_BEFORE_CLOSE)
		{
			std::cout << "Delais trop long deco de la socket : " << temp->first << std::endl;
			delEpollEvent(epollFd, const_cast<int &>(temp->first));//aaaaa
			close(temp->first);
			timer.erase(temp->first);
		}
	}
	return (epoll_wait(epollFd, events.data(), events.size(), -1));
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

void	addPlaceEventLog(int nfds, std::vector<struct epoll_event> &events)
{
	if (nfds == static_cast<int>(events.size()))
	{
		std::cout << "Redim tab evenement add place. Taille now = "<< events.size() * 2 << std::endl;
		std::cout << std::endl;
		events.resize(nfds * 2);
	}
}

void	delPlaceEventLog(int nfds, std::vector<struct epoll_event> &events)
{
	if (nfds < static_cast<int>(events.size()) / 4 && events.size() > NB_EVENT_BASE)
	{
		std::cout << "Redim tab evenement delete place. Taille now = " << events.size() / 2 << std::endl;
		std::cout << std::endl;
		events.resize(nfds / 2);
	}
}