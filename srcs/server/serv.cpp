#include "../../include/serveur.hpp"

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		t_server data;
		data.epollFd = epoll_create1(0);
		std::vector<struct epoll_event> events(NB_EVENT_BASE);

		/*Config serveur*/
		if (parsingConf(data, argv) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			return (1);
		}
		/*Fin config serveur*/
		memset(events.data(), 0, sizeof(events));
		initAdresse(data);
		data.serverSocket = initSocket(data);
		while(1)
		{
			//Pour serveur vituelle map avec num socketserveur et structure avec toute les autres info
			int nfds = checkTimeAndWaitPoll(data, events);
			addPlaceEventLog(data.epollFd, events);
			for (int i = 0; i < nfds; i++)
			{
				if (events[i].data.fd == data.serverSocket)//C'est la socket serveur donc on recoit un nouveau client
					acceptNewClient(data);
				else if (events[i].events & EPOLLRDHUP)//Absolument devant EPOLLIN
					disconnectClient(data, events[i].data.fd);
				else if (events[i].events & EPOLLIN)
					manageClient(data, events[i].data.fd);
				else if (events[i].events & EPOLLERR)
					errorClient(data, events[i].data.fd);
			}
			delPlaceEventLog(data.epollFd, events);
		}
		close(data.serverSocket);
	}
	else
		std::cout << "Incorrect argument number" << std::endl;
}