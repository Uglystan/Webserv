#include "../../include/serveur.hpp"
#include "../response/response.hpp"

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

void	manageClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer)
{
	char	buffer[1025];
	int	bytes_read = 0;
	std::string	msg;

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
		msg.append(buffer);
		if (bytes_read == 1024)
		{
			while (bytes_read == 1024)
			{
				memset(buffer, 0, sizeof(buffer));
				bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);//proteger recv encore ?
				msg.append(buffer);
				if (msg.size() > MAX_CLIENT_BODYSIZE)
				{
					/*Error page 413*/
				}
			}
		}
		std::cout << "Message recu : " << msg << std::endl;
		Response resp(msg);
		std::string rep = resp.create_response();
		std::cout << "Message envoyee : " << rep << std::endl;
		send(clientSocket, rep.c_str(), rep.size(), 0);
	}
}

void	disconnectClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Deconnexion client" << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}

void	errorClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer)
{
	std::cout << "Error sur socket" << std::endl;
	timer.erase(socket);
	delEpollEvent(epollFd, socket);
	close(socket);
}