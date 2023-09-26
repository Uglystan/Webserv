#include "../../include/serveur.hpp"

void	execCgi(std::string &msg, int &clientSocket)
{
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
		if (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
			std::cout << "Error dup2 fils ou close fd[0] fils ou close fd[1] fils" << std::endl;
		const char *program = "srcs/cgi/a.out";
		char *const av[] = {(char *)program, (char *)msg.c_str(), NULL};
		execve(program, av, environ);
		std::cerr << "Error execve" << std::endl;
	}
	waitpid(pid, &status, 0);
	int gnlfd = open("srcs/server/test", O_RDONLY, O_WRONLY,O_TRUNC);
	if (gnlfd == -1 || dup2(fd[0], gnlfd) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
		std::cout << "Error opening file parent ou dup2 parent ou close fd[0] parent ou close fd[1] parent" << std::endl;
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