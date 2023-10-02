#include "../../include/serveur.hpp"

int	checkServ(t_server data, std::vector<struct epoll_event> events, int i)
{
	for (unsigned int j = 0; j < data.config.size(); j++)
	{
		if (events[i].data.fd == data.config[j].serverSocket)//C'est la socket serveur donc on recoit un nouveau client
		{
			return (data.config[j].serverSocket);
		}
	}
	return (-1);
}

int main (int argc, char **argv)
{
	if (argc == 2)
	{
		t_server data;
		data.epollFd = epoll_create1(0);
		std::vector<struct epoll_event> events(NB_EVENT_BASE);

		if (parsingConf(data, argv) == -1)
		{
			std::cerr << strerror(errno) << std::endl;
			return (1);
		}
		memset(events.data(), 0, sizeof(events));
		initAllServ(data);
		while(1)
		{
			int nfds = checkTimeAndWaitPoll(data, events);
			addPlaceEventLog(data.epollFd, events);
			for (int i = 0; i < nfds; i++)
			{
				int s = checkServ(data, events, i);
				if (s != -1)//C'est la socket serveur donc on recoit un nouveau client
					acceptNewClient(data, s);
				else if (events[i].events & EPOLLRDHUP)//Absolument devant EPOLLIN
					disconnectClient(data, events[i].data.fd);
				else if (events[i].events & EPOLLIN)
					manageClient(data, events[i].data.fd);
				else if (events[i].events & EPOLLERR)
					errorClient(data, events[i].data.fd);
			}
			delPlaceEventLog(data.epollFd, events);
		}
		//close tout les serveurs
	}
	else
		std::cout << "Incorrect argument number" << std::endl;
}