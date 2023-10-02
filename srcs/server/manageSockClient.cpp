#include "../../include/serveur.hpp"

void	acceptNewClient(t_server &data, int &serverSocket)
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
		data.timer[clientSocket] = time;
		addEpollEvent(data.epollFd, clientSocket);
	}
}

void	manageClient(t_server &data, int &clientSocket)
{
	char	buffer[1024];
	size_t	sizeHeader = 0;

	memset(buffer, 0, sizeof(buffer));
	data.req[clientSocket].bytes += recv(clientSocket, buffer, 1024, MSG_DONTWAIT);
	perror("ERR");
	data.req[clientSocket].message += std::string(buffer);
	sizeHeader = data.req[clientSocket].message.find("\r\n\r\n");
	if (data.req[clientSocket].bytes == -1 || data.req[clientSocket].bytes == 0)
	{
		if (data.req[clientSocket].bytes == -1)
			std::cout << "Error recv" << std::endl;
		else
			std::cout << "Client deco" << std::endl;
		data.req.erase(clientSocket);
		data.timer.erase(clientSocket);
		delEpollEvent(data.epollFd, clientSocket);
		close(clientSocket);
	}
	else if (data.req[clientSocket].bytes > 0)
	{
		gettimeofday(&data.timer[clientSocket], NULL);//reset
		if (data.req[clientSocket].message.find("Content-Length: ") != std::string::npos)//Post en plusieur morceau
			recvMulti(data, clientSocket, sizeHeader);
		else if (data.req[clientSocket].message.find("Content-Length: ") == std::string::npos)
		{
			if (data.req[clientSocket].message.find("Transfer-Encoding") == std::string::npos && data.req[clientSocket].message.size() >= sizeHeader)//Get base
				recvBase(data, clientSocket);
			else if (data.req[clientSocket].message.find("Transfer-Encoding") != std::string::npos)//Requete chunk
				recvChunk(data, clientSocket);
		}
	}
}

void	disconnectClient(t_server &data, int &socket)
{
	std::cout << "Deconnexion socket : " << socket << std::endl;
	data.timer.erase(socket);
	delEpollEvent(data.epollFd, socket);
	close(socket);
}

void	errorClient(t_server &data, int &socket)
{
	std::cout << "Error sur socket : " << socket << std::endl;
	data.timer.erase(socket);
	delEpollEvent(data.epollFd, socket);
	close(socket);
}
