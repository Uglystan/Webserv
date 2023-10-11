#include "cgi_handler.hpp"
extern char **environ;

static void writeReqBody(std::string requestbody, std::string size)
{
	//(void)size;
	int script_input[2];
	pipe(script_input);
	dup2(script_input[0], 0);
	close(script_input[0]);
	int ret = write(script_input[1], requestbody.c_str(), atoi(size.c_str()));// Écrire le contenu de la chaîne de caractères "request" dans le tube.
	std::cerr << "YPPPPPPPPPPPPPPPPPPP" << std::endl;
	std::cout << ret << std::endl;
	close(script_input[1]);
	// std::ofstream fichier("/mnt/nfs/homes/abourdon/Desktop/Webserv/srcs/http/cgi_handler/temp");
	// if (fichier.is_open())
	// {
	// 	fichier << requestbody;
	// 	fichier.close();
	// 	std::cout << "Contenu écrit dans le fichier avec succès." << std::endl;
	// }
	// else
	// 	std::cerr << "Impossible d'ouvrir le fichier : " << std::endl;
}

static int	cgi_path_access(std::string &cgipath)
{
	if (access(cgipath.c_str(), X_OK) == 0)
		return (0);
	else
		return (1);
}

std::string	execCgi(std::string path, std::string	_requestbody, std::string size, std::string &cgi_path)
{
	std::string	reponse;
	std::string	line;
	int	status;
	int	fd[2];
	int	pid;
	if (cgi_path_access(cgi_path) == 1)
		return ("");
	std::string requestMethod = std::getenv("REQUEST_METHOD");
	if (pipe(fd) == -1)
		std::cout << "Error pipe" << std::endl;
	pid = fork();
	if (pid == -1)
		std::cout << "Error fork" << std::endl;
	if (!pid)
	{
		if (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1)
			std::cerr << "Error dup2 fils ou close fd[0] fils ou close fd[1] fils" << std::endl;
		if (requestMethod == "POST")
			writeReqBody(_requestbody, size);
		const char *program = cgi_path.c_str();
		char *const av[] = {(char *)program, (char *)path.c_str(), NULL};
		execve(program, av, environ);
		std::cerr << "Error execve" << std::endl;
	}
	if (close(fd[1]) == -1)
	 	std::cout << "Error opening file parent ou dup2 parent ou close fd[0] parent ou close fd[1] parent" << std::endl;
	waitpid(pid, &status, 0);
	char buff[4096];
	bzero(buff, 4096);
	while (1)
	{
		ssize_t bytesRead = read(fd[0], buff, sizeof(buff) - 1);
		if (bytesRead <= 0)
			break;
		buff[bytesRead] = '\0';
		reponse.append(buff);
		bzero(buff, 4096);
	}
	if (close(fd[0]) == -1)
		std::cout << "Error closing fd[0]" << std::endl;
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		std::cerr << "Script error \n";
	size_t finddoctype = reponse.find("<");
	if (finddoctype == std::string::npos)
		return ("");
	return (reponse.substr(finddoctype));
}