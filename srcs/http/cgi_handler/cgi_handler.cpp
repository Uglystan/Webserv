#include "cgi_handler.hpp"
extern char **environ;

static int writeReqBody(std::string requestbody)
{
	std::ofstream fichier("srcs/http/cgi_handler/temp", std::ifstream::trunc | std::ifstream::out | std::ifstream::in);
	fichier << requestbody;
	int fd = ::open("srcs/http/cgi_handler/temp", O_RDONLY);
	if (fd != -1)
	{
		dup2(fd, 0);
		close(fd);
	}
	else
		return (1);
	if (remove("srcs/http/cgi_handler/temp") != 0)
		return (1);
	return(0);
}

static int	cgi_path_access(std::string &cgipath)
{
	if (access(cgipath.c_str(), X_OK) == 0)
		return (0);
	else
		return (1);
}

std::string	execCgi(std::string path, std::string	_requestbody, std::string &cgi_path)
{
	std::string	reponse;
	std::string	line;
	int	status;
	int	pid;
	time_t startTime = time(NULL);
	if (cgi_path_access(cgi_path) == 1)
		return ("");
	std::string requestMethod = std::getenv("REQUEST_METHOD");
	pid = fork();
	if (pid == -1)
		return ("");
	std::ifstream fichier("srcs/http/cgi_handler/pipe", std::ifstream::trunc | std::ifstream::out | std::ifstream::in);
	int fd = ::open("srcs/http/cgi_handler/pipe", O_RDONLY | O_WRONLY);
	if (fd == -1)
		return ("");
	if (!pid)
	{
		if (dup2(fd, 1) == -1)
			return ("");
		if (requestMethod == "POST")
		{
			if (writeReqBody(_requestbody) == 1)
				return("");
		}
		const char *program = cgi_path.c_str();
		char *const av[] = {(char *)program, (char *)path.c_str(), NULL};
		execve(program, av, environ);
		std::cerr << "Error execve" << std::endl;
		return ("");
	}
	while (1)
	{
		time_t	currentTime = time(NULL);
		if (currentTime - startTime >= 5)
			kill(pid, SIGKILL);
		if (waitpid(pid, &status, WNOHANG) == pid)
			break ;
	}
	std::string buffer;
	while (std::getline(fichier, buffer))
	{
		if (!fichier.eof())
			reponse.append(buffer + '\n');
		else
			reponse.append(buffer);
	}
	if (close(fd) == -1)
		return ("");
	if (remove("srcs/http/cgi_handler/pipe") != 0)
			return ("");
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		if (status == 512)
			return ("-2");
		std::cerr << "Script error \n";
		return ("-1");
	}
	size_t finddoctype = reponse.find("<");
	if (finddoctype == std::string::npos)
		return ("");
	return (reponse.substr(finddoctype));
}