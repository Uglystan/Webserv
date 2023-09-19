#include <sys/socket.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_CLIENT 100

void	createSocketListening(struct sockaddr_in *clientAddr, struct pollfd *fds)
{
	fds[0].fd = socket(AF_INET, SOCK_STREAM, 0);
	fds[0].events = POLLIN;
	if (fds[0].fd < 0)
	{
		std::cout << "Throw exception" << std::endl;
		/*throw exception*/
	}
	clientAddr[0].sin_family = AF_INET;
	clientAddr[0].sin_port = htons(8080);
	clientAddr[0].sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(fds[0].fd, (struct sockaddr *) &clientAddr[0], (socklen_t)sizeof(clientAddr[0])) < 0)
	{
		std::cout << "Error bind" << std::endl;
		/*throw exception*/
	}
	if (listen(fds[0].fd, 127) < 0)
	{
		std::cout << "Error listen" << std::endl;
		/*throw exception*/
	}
}

std::string	readSocket(struct pollfd *fds, int i, struct sockaddr_in *clientAddr)
{
	std::string	msgRecu;
	int	bytes_read = 0;
	char	buffer[100];

	while ((bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0)) > 0)
	{
		msgRecu.append(buffer, bytes_read);
	}
	std::cout << msgRecu << std::endl;
	if (bytes_read == 0)
	{
		std::cout << "Donnees recue" << std::endl;
		close(fds[i].fd);
		fds[i].fd = -1;
		memset(&clientAddr[i], 0, sizeof(struct sockaddr_in));
	}
	if (bytes_read < 0)
	{
		std::cout << "Error recv" << std::endl;
		/*throw exception*/
	}
	return (msgRecu);
}

void	acceptSocket(struct pollfd *fds, struct sockaddr_in *clientAddr)
{
	int clientSock;
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (fds[i].fd == -1)
		{
			int test = sizeof(clientAddr[i]);
			clientSock = accept(fds[0].fd, (struct sockaddr *) &clientAddr[i], (socklen_t *)&test);
		}
	}
	if (clientSock < 0)
	{
		std::cout << "Error accept" << std::endl;
		/*throw exception*/
	}
	int flags = fcntl(clientSock, F_GETFL, 0);
	fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
	for (int i = 1; i < MAX_CLIENT; i++)
	{
		if (fds[i].fd == -1)
		{
			fds[i].fd = clientSock;
			fds[i].events = POLLIN;
			break;
		}
	}
}

int main (/*File conf*/)
{
	struct sockaddr_in	clientAddr[MAX_CLIENT];
	std::string	msgRecu;
	struct pollfd	fds[MAX_CLIENT];

	signal(SIGPIPE, SIG_IGN);//Ignore les signaux brokepipe (quand un client quitte la connexion sinon programme s'arrete avec erreur 141)
	memset(fds, -1, sizeof(struct pollfd));
	memset(clientAddr, 0, sizeof(struct sockaddr_in));

	createSocketListening(clientAddr, fds);
	while (1)
	{
		int result = poll(fds, MAX_CLIENT + 1, -1);
		if (result > 0)
		{
			for (int i = 1; i < MAX_CLIENT + 1; i++)
			{
				if (fds[i].revents & POLLIN)
				{
					msgRecu = readSocket(fds, i, clientAddr);
					//Parsing de requete client
					//Analyser requete
					//Preparer reponse
					//Envoie reponse
					// if (send(fds[i].fd, message.c_str(), message.size(), 0) < 0)
					// {
					// 	std::cerr << "Error send" << std::endl;
					// 	/*throw exception*/
					// }
					//close socket une fois rep envoyee
				}
				if (fds[i].revents & POLLHUP)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					memset(&clientAddr[i], 0, sizeof(struct sockaddr_in));
					std::cerr << "Connexion fermee par le client" << std::endl;
				}
				if (fds[i].revents & POLLERR)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
					memset(&clientAddr[i], 0, sizeof(struct sockaddr_in));
					std::cerr << "Connexion stop sans raison" << std::endl;
				}
			}
			if (fds[0].revents & POLLIN)
				acceptSocket(fds, clientAddr);
				//close (clientSock);
		}
	//close (servSock);
	}
}