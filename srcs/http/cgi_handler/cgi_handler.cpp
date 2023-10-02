#include "cgi_handler.hpp"
extern char **environ;

void writeReqBody(std::string requestbody)
{
	int script_input[2];
	pipe(script_input);
	dup2(script_input[0], 0);
	close(script_input[0]);
	write(script_input[1], requestbody.c_str(), requestbody.size());// Écrire le contenu de la chaîne de caractères "request" dans le tube.
	close(script_input[1]);
}

std::string	execCgi(std::string path, std::string	_requestbody)
{
	std::string	reponse;
	std::string	line;
	int	status;
	int	fd[2];
	int	pid;
	std::string requestMethod = std::getenv("REQUEST_METHOD");
	if (requestMethod == "POST")
		writeReqBody(_requestbody);
	if (pipe(fd) == -1)
		std::cout << "Error pipe" << std::endl;
	pid = fork();
	if (pid == -1)
		std::cout << "Error fork" << std::endl;
	if (!pid)
	{
		if (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
			std::cerr << "Error dup2 fils ou close fd[0] fils ou close fd[1] fils" << std::endl;
		const char *program = "/usr/bin/php-cgi";
		char *const av[] = {(char *)program, (char *)path.c_str(), NULL};
		execve(program, av, environ);
		std::cerr << "Error execve" << std::endl;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		std::cerr << "SCRIPT ERRRRRRRRRRRRRRRRRRRRROR\n";
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
	size_t finddoctype = reponse.find("<!DOCTYPE");
	if (finddoctype == std::string::npos)
		return ("");
	return (reponse.substr(finddoctype));
}