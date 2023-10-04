#include "cgi_handler.hpp"
extern char **environ;

void writeReqBody(std::string requestbody, std::string size)
{
	int script_input[2];
	pipe(script_input);
	dup2(script_input[0], 0);
	close(script_input[0]);
	//std::cout << "Size : " << atoi(size.c_str()) << std::endl;
	//std::cout << "RequestSize : " << requestbody.size() << std::endl;
	int ret = write(script_input[1], requestbody.c_str(), atoi(size.c_str()));// Écrire le contenu de la chaîne de caractères "request" dans le tube.
	std::cout << ret << std::endl;
	close(script_input[1]);
}

std::string	execCgi(std::string path, std::string	_requestbody, std::string size)
{
	std::string	reponse;
	std::string	line;
	int	status;
	int	fd[2];
	int	pid;
	std::string requestMethod = std::getenv("REQUEST_METHOD");
	// if (requestMethod == "POST")
	// 	writeReqBody(_requestbody, size);
	if (pipe(fd) == -1)
		std::cout << "Error pipe" << std::endl;
	pid = fork();
	if (pid == -1)
		std::cout << "Error fork" << std::endl;
	if (!pid)
	{
		if (requestMethod == "POST")
			writeReqBody(_requestbody, size);
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
	//int gnlfd = open("srcs/server/test", O_RDONLY, O_WRONLY,O_TRUNC);
	if (close(fd[1]) == -1)
	 	std::cout << "Error opening file parent ou dup2 parent ou close fd[0] parent ou close fd[1] parent" << std::endl;
	// char *t = (char*)(malloc(sizeof(char) * 1));
	// t[0] = '\0';
	// while (1)
	// {
	// 	if ((t = get_next_line(fd[0])) == NULL)
	// 		break ;
	// 	reponse.append(t);
	// 	free (t);
	// }
	std::string bufferStr;
	char buff[1024];
	bzero(buff, 1024);
	while (read(fd[0], buff, 1023) > 0)
	{
		bufferStr.append(buff);
		bzero(buff, 1024);
	}
	std::cout << "ARGUIUUUUUUUUUUUUUUUUUUUUUUUUUUU\n" <<bufferStr<< std::endl;
	if (close(fd[0]) == -1)
		std::cout << "Error closing fd[0]" << std::endl;
	reponse = bufferStr;
	size_t finddoctype = reponse.find("<!DOCTYPE");
	if (finddoctype == std::string::npos)
		return ("");
	return (reponse.substr(finddoctype));
}