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

std::string	readClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer, int &bytes_read)
{
	char	buffer[1025];
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
	}
	return (msg);
}

void	manageClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer)
{
	int	bytes_read = 0;
	std::string	msg = readClient(epollFd, clientSocket, timer, bytes_read);
	// std::string	reponse;
	// std::string	line;
	// int	status;
	// int	fd[2];
	// int	pid;

	if (bytes_read > 0)
	{
		std::cout << "Message recu : " << msg << std::endl;
		/*Ici fork pour appeller le CGI*/
		// if (pipe(fd) == -1)
		// 	std::cout << "Error pipe" << std::endl;
		// pid = fork();
		// if (pid == -1)
		// 	std::cout << "Error fork" << std::endl;
		// if (!pid)
		// {
		// 	if (dup2(fd[1], 1) == -1)
		// 		std::cout << "Error dup2 fils" << std::endl;
		// 	if (close(fd[0]) == -1)
		// 		std::cout << "Error close fd[0] fils" << std::endl;
		// 	if (close(fd[1]) == -1)
		// 		std::cout << "Error close fd[1] fils" << std::endl;
		// 	execve(/*Programme alex avec msg comme argument*/);
		// 	std::cout << "Error execve" << std::endl;
		// }
		// waitpid(pid, &status, 0);
		// if (dup2(fd[0], /*Dans un fichier*/) == -1)
		// 		std::cout << "Error dup2 parent" << std::endl;
		// if (close(fd[0]) == -1)
		// 	std::cout << "Error close fd[0] parent" << std::endl;
		// if (close(fd[1]) == -1)
		// 	std::cout << "Error close fd[1] parent" << std::endl;
		// std::ifstream 	flux_reponse("/*nom du fichier*/");//Nom fichier ici
		// if (flux_reponse.is_open() == -1)
		// 	std::cout << "Error ifstream open" << std::endl;
		// while (std::getline(flux_reponse, line))
		// {
		// 	reponse.append(line);
		// 	reponse.append("\n");
		// }
		/*Fin du fork*/
		Response resp(msg);
		std::string rep = resp.create_response();
		//std::cout << "Message envoyee : " << rep << std::endl;
		send(clientSocket, rep.c_str(), rep.size(), 0);//send reponse
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