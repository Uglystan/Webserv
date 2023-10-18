#include "../../include/serveur.hpp"

/*Quand on a une activite sur une socket on parcours tout les serveurs pour savoir lequel a ete "active" en fonction de la socket serveur*/
int	checkEventServ(t_server data, std::vector<struct epoll_event> events, int i)
{
	for (unsigned int j = 0; j < data.config.size(); j++)
	{
		if (events[i].data.fd == data.config[j].serverSocket)
		{
			std::cout << "Activite sur la socket serveur : " << events[i].data.fd << std::endl;
			return (data.config[j].serverSocket);
		}
	}
	return (-1);
}

void	*ft_memset(void *dest, int c, size_t size)
{
	size_t	i;
	char	*array;

	array = (char *) dest;
	i = 0;
	while (i < size)
	{
		array[i] = c;
		i++;
	}
	return (array);
}

int main (int argc, char **argv)
{
	t_server data;
	data.epollFd = epoll_create1(0);
	std::vector<struct epoll_event> events(NB_EVENT_BASE);

	if (parsingConf(data, argv, argc) == -1)
		return (1);
	ft_memset(events.data(), 0, sizeof(events));
	if (initAllServ(data) == -1)
		return (1);
	while(1)
	{
		try
		{
			int nfds = checkTimeAndWaitPoll(data, events);
			addPlaceEventLog(data.epollFd, events);
			for (int i = 0; i < nfds; i++)
			{
				int s = checkEventServ(data, events, i);
				if (s != -1)
					acceptNewClient(data, s);
				else if ((events[i].events & EPOLLRDHUP) || events[i].events & EPOLLERR)//Absolument devant EPOLLIN
					disconnectClient(data, events[i].data.fd);
				else if (events[i].events & EPOLLIN)
					manageClient(data, events[i].data.fd);
			}
			delPlaceEventLog(data.epollFd, events);
		}
		catch (errorContinueServ const& e)
		{
			errorContinue(data, e);
		}
		catch (errorStopServ const& e)
		{
			errorStop(data, e);
			break;
		}
	}
}
