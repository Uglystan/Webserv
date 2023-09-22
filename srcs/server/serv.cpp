#include "../../include/serveur.hpp"

int main (/*File conf*/)
{
	int epollFd = epoll_create1(0);
	std::vector<struct epoll_event> events(NB_EVENT_BASE);
	struct sockaddr_in	adresse;
	std::map <int, struct timeval>	timer;

	memset(events.data(), 0, sizeof(events));
	initAdresse(adresse);
	int serverSocket = initSocket(adresse, epollFd);
	while(1)
	{
		int nfds = checkTimeAndWaitPoll(epollFd, events, timer);
		addPlaceEventLog(epollFd, events);
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
		delPlaceEventLog(epollFd, events);
	}
	close(serverSocket);
}