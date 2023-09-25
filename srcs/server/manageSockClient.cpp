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
		//std::cout << "Message recu : " << msg << std::endl;
		std::string	reponse;
		std::string	line;
		int	status;
		int	fd[2];
		int	pid;

		/*Ici fork pour appeller le CGI*/
		if (pipe(fd) == -1)
			std::cout << "Error pipe" << std::endl;
		pid = fork();
		if (pid == -1)
			std::cout << "Error fork" << std::endl;
		if (!pid)
		{
			if (dup2(fd[1], 1) == -1)
				std::cout << "Error dup2 fils" << std::endl;
			if (close(fd[0]) == -1)
				std::cout << "Error close fd[0] fils" << std::endl;
			if (close(fd[1]) == -1)
				std::cout << "Error close fd[1] fils" << std::endl;
			const char *program = "srcs/cgi/a.out";
			char *const av[] = {(char *)program, (char *)msg.c_str(), NULL};
			//std::cerr << "before execve: \n" << av[1] << std::endl;
			execve(program, av, environ);
			std::cerr << "Error execve" << std::endl;
		}
		waitpid(pid, &status, 0);
		//std::stringstream output_stream;
		int gnlfd = open("/mnt/nfs/homes/abourdon/Desktop/Webserv/srcs/server/test", O_RDONLY, O_WRONLY,O_TRUNC);
		if (gnlfd == -1)
			std::cout << "Error opening file" << std::endl;
		if (dup2(fd[0], gnlfd) == -1)
				std::cout << "Error dup2 parent" << std::endl;
		if (close(fd[0]) == -1)
			std::cout << "Error close fd[0] parent" << std::endl;
		if (close(fd[1]) == -1)
			std::cout << "Error close fd[1] parent" << std::endl;
		char *t = (char*)(malloc(sizeof(char) * 1));
		t[0] = '\0';
		while (1)
		{
			if ((t = get_next_line(gnlfd)) == NULL)
				break ;
			reponse.append(t);
			free (t);
		}
		if (close(gnlfd) == -1)
			 std::cout << "Error closing gnlfd" << std::endl;
		std::cout << "Message envoyee : " << reponse << std::endl;
		send(clientSocket, reponse.c_str(), reponse.size(), 0);
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
