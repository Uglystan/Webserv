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

void	createSocketListening(struct sockaddr_in &servAddr, struct pollfd *fds)
{
	fds[0].fd = socket(AF_INET, SOCK_STREAM, 0);
	fds[0].events = POLLIN;
	if (fds[0].fd < 0)
	{
		std::cout << "Throw exception" << std::endl;
		/*throw exception*/
	}
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8080);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(fds[0].fd, (struct sockaddr *) &servAddr, (socklen_t)sizeof(servAddr)) < 0)
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

std::string	readSocket(struct pollfd *fds, int i)
{
	std::string	msgRecu;
	int	bytes_read = 0;
	char	buffer[100];

	while ((bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0)) > 0)
	{
		msgRecu.append(buffer, bytes_read);
	}
	std::cout << msgRecu << std::endl;
	if (bytes_read < 0)
	{
		std::cout << "Error recv" << std::endl;
		/*throw exception*/
	}
	return (msgRecu);
}

void	acceptSocket(struct pollfd *fds, struct sockaddr_in &servAddr, int &nbrClient)
{
		int clientSock =  0;
		int test = sizeof(servAddr);
		clientSock = accept(fds[0].fd, (struct sockaddr *) &servAddr, (socklen_t *)&test);
		int flags = fcntl(clientSock, F_GETFL, 0);
		fcntl(clientSock, F_SETFL, flags | O_NONBLOCK);
		fds[nbrClient].fd = clientSock;
		//fds[nbrClient].fd = accept(fds[0].fd, (struct sockaddr *) &servAddr, (socklen_t *)&test);
		if (fds[nbrClient].fd < 0)
		{
			std::cout << "Error accept" << std::endl;
			/*throw exception*/
		}
		fds[nbrClient].events = POLLIN;
		nbrClient++;
}

int main (/*File conf*/)
{
	struct sockaddr_in	servAddr;
	std::string	message = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<!DOCTYPE html>\n<html>\n<head>\n\t<title>Page d'exemple</title>\n</head>\n<body>\n\t<h1>Bienvenue sur la page d'exemple</h1>\n</body>\n</html>\n\n";
	std::string	msgRecu;
	struct pollfd	fds[100];
	int nbrClient = 1;//1 car 0 pris par socket d'ecoute

	signal(SIGPIPE, SIG_IGN);

	createSocketListening(servAddr, fds);
	while (1)
	{
		int result = poll(fds, nbrClient + 1, -1);
		if (result <= 0)
		{
			std::cout << "Throw exception" << std::endl;
			/*throw exception*/
		}
		for (int i = 1; i < nbrClient; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				std::cout << i << std::endl;
				msgRecu = readSocket(fds, i);
				//Parsing de requete client
				//Analyser requete
				//Preparer reponse
				//Envoie reponse
				if (send(fds[i].fd, message.c_str(), message.size(), 0) < 0)
				{
					if (errno == EPIPE || errno == ECONNRESET)//gestion d'erreur si le client ferme la co ou reinitilise avant de tout recevoir
					{
						close(fds[i].fd);
						fds[i].fd = -1;
					}
					std::cerr << "Error send" << std::endl;
					/*throw exception*/
				}
			}
		}
		if (fds[0].revents & POLLIN)
			acceptSocket(fds, servAddr, nbrClient);
			//close (clientSock);
	}
	//close (servSock);
}