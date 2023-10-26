#include "../../include/serveur.hpp"

/*Ici la premiere fonction sert a regarder si une socket a atteint sont timeout si oui on la supprime de toute les map etc..
et ensuite on attend qu'une socket soit active*/

/*La 2eme et 3eme fonctions servent ajouter ou supprimer une socket de la gestion de epoll*/

/*Et les 2 dernieres vont augmenter ou baisser la taille du journal d'evenement de socket en fonction du trafic sur le site*/

int	checkTimeAndWaitPoll(t_server &data, std::vector<struct epoll_event> &events)
{
	struct timeval timeNow;
	gettimeofday(&timeNow, NULL);
	std::map<int, struct timeval>::iterator i = data.timer.begin();
	std::map<int, struct timeval>::iterator temp = i;

	while (i != data.timer.end())
	{
		temp = i;
		i++;
		if ((timeNow.tv_sec - temp->second.tv_sec) + (timeNow.tv_usec - temp->second.tv_usec) / 1000000 > MAX_TIME_BEFORE_CLOSE)
		{
			//std::cout << "Delais timeout atteint deconnexion de la socket : " << temp->first << std::endl;
			delEpollEvent(data.epollFd, const_cast<int &>(temp->first));//aaaaa
			if (close(temp->first) == -1)
				throw errorStopServ(strerror(errno));
			data.req.erase(temp->first);
			data.timer.erase(temp->first);
		}
	}
	int ret = epoll_wait(data.epollFd, events.data(), events.size(), -1);
	if (ret == -1)
		throw errorStopServ(strerror(errno));
	return (ret);
}

void	delEpollEvent(int &epollFd, int &socket)
{
	struct epoll_event event;

	ft_memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	event.events = EPOLLIN | EPOLLRDHUP;
	if (epoll_ctl(epollFd, EPOLL_CTL_DEL, socket, &event) == -1)
		throw errorStopServ(strerror(errno));
	//std::cout << "Suppression du journal d'evenement de la socket : " << socket << std::endl;	
}

//Fonction pour ajouter des socket aux evenement de Epool
void	addEpollEvent(int &epollFd, int &socket)
{
	//Ici la structure sert juste setup les evenements pour la socket et a l'ajouter a la liste d'evenement
	struct epoll_event event;
	ft_memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	event.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socket, &event) == -1)
		throw errorStopServ(strerror(errno));
	//std::cout << "Ajout au journal d'evenement de la socket : " << socket << std::endl;
	
}

void	addPlaceEventLog(int nfds, std::vector<struct epoll_event> &events)
{
	if (nfds == static_cast<int>(events.size()))
	{
		events.resize(nfds * 2);
	}
}

void	delPlaceEventLog(int nfds, std::vector<struct epoll_event> &events)
{
	if (nfds < static_cast<int>(events.size()) / 4 && events.size() > NB_EVENT_BASE)
	{
		events.resize(nfds / 2);
	}
}