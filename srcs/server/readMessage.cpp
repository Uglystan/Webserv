#include "../../include/serveur.hpp"

void	recvMessage(int &bytes_read, int &clientSocket, char *buffer)
{
	std::string	msg;
	int	totalBytesRead = 0;
	int	sizeMessageTotal = sizeMessage(buffer);

	msg.append(buffer);
	totalBytesRead += bytes_read;
	while (totalBytesRead != sizeMessageTotal)//avant y'avait "<" et ca marchais !!!!!!!!!!!!!!!!!!!!!!
	{
		memset(buffer, 0, sizeof(&buffer));
		bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);//proteger recv encore ?
		msg.append(buffer);
		if (bytes_read != -1)
			totalBytesRead += bytes_read;
	}
	//debut fork
	std::string	reponse;
	std::string	line;
	int	status;
	int	fd[2];
	int	pid;
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
	int gnlfd = open("srcs/server/test", O_RDONLY, O_WRONLY,O_TRUNC);
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
	// std::cout << "Message recu : " << msg << std::endl;
	// std::string rep = resp.find_method();
	// send(clientSocket, rep.c_str(), rep.size(), 0);
}