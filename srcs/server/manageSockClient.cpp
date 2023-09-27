#include "../../include/serveur.hpp"

void	acceptNewClient(t_server &data)
{
	int clientSocket;
	struct sockaddr_in clientAdresse;
	int addresseLen = sizeof(clientAdresse);
	struct timeval time;

	clientSocket = accept(data.serverSocket, (struct sockaddr*) &clientAdresse, (socklen_t *) &addresseLen);
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
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Faire une map avec une structure associe au fd socket et dans la structure la string et le nbr de bytes
	memset(buffer, 0, sizeof(buffer));
	data.bytes[clientSocket] += recv(clientSocket, buffer, 1024, MSG_DONTWAIT);
	data.request[clientSocket] += std::string(buffer);
	sizeHeader = data.request[clientSocket].find("\r\n\r\n");
	if (data.bytes[clientSocket] == -1 || data.bytes[clientSocket] == 0)
	{
		if (data.bytes[clientSocket] == -1)
			std::cout << "Error recv" << std::endl;
		else
			std::cout << "Client deco" << std::endl;
		data.bytes.erase(clientSocket);
		data.request[clientSocket].erase();
		data.timer.erase(clientSocket);
		delEpollEvent(data.epollFd, clientSocket);
		close(clientSocket);
	}
	else if (data.bytes[clientSocket] > 0)
	{
		gettimeofday(&data.timer[clientSocket], NULL);//reset
		if (data.request[clientSocket].find("Content-Length: ") != std::string::npos)//Post en plusieur morceau
			recvMulti(data, clientSocket, sizeHeader);
		else if (data.request[clientSocket].find("Content-Length: ") == std::string::npos)
		{
			if (data.request[clientSocket].find("Transfer-Encoding") == std::string::npos && data.request[clientSocket].size() >= sizeHeader)//Get base
				recvBase(data, clientSocket);
			else if (data.request[clientSocket].find("Transfer-Encoding") != std::string::npos)//Requete chunk
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
