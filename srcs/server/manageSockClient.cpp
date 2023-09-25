#include "../../include/serveur.hpp"

void	acceptNewClient(int &serverSocket, std::map<int, struct timeval> &timer, int &epollFd)
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
		addEpollEvent(epollFd, clientSocket);
	}
}

int	is_chunked(char *buffer)
{
	std::string	buf = buffer;
	if (buf.find("Transfer-Encoding: chunked") != std::string::npos)
		return(1);
	else
		return (0);
}

void	manageClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer)
{
	char	buffer[1024];
	int	bytes_read = 0;

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
		if (is_chunked(buffer) == 1)
		{
		}
		else
			recvMessage(bytes_read, clientSocket, buffer);
	}
}

void	disconnectClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Deconnexion socket : " << socket << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}

void	errorClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Error sur socket : " << socket << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}
