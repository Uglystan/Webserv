#pragma once

#include <sys/socket.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <fstream>
#include <cstdio>
#include <sys/types.h>
#include <map>
#include <sys/time.h>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <regex.h>

#define NB_EVENT_BASE 100
#define MAX_TIME_BEFORE_CLOSE 300

typedef struct s_requestData
{
	int	bytes;
	std::string	message;
}t_requestData;

typedef struct s_location
{
	std::string	directory;
	std::string	root;
	std::string	allow_methods;
	std::string	redirection;
}t_location;

typedef struct s_configServ
{
	int	serverSocket;
	std::string	serverName;
	std::string	ip;
	int	port;
	std::string	root;
	std::string	index;
	std::string	errorPage;
	int	maxBodySize;
	std::string	cgi;
	std::string	cgiExt;
	std::string	allowMethods;
	std::string	tmp;
	std::string	listening_file;
	std::vector<t_location>	locationVec;
}t_configServ;

typedef struct s_server
{
	int epollFd;
	struct sockaddr_in	adresse;
	std::map <int, struct timeval>	timer;
	std::map <int, t_requestData> req;
	std::vector <t_configServ> config;
	int	bytes_read;

}t_server;

class errorStopServ
{
	public :
		errorStopServ(std::string codeError)
		{
			_codeError = codeError;
		}
		~errorStopServ()
		{
			
		}
		std::string	getCodeError() const
		{
			return (_codeError);
		}
	private :
		std::string	_codeError;
};

class errorContinueServ
{
	public :
		errorContinueServ(std::string codeError, t_configServ serv, int clientSocket)
		{
			_codeError = codeError;
			_serv = serv;
			_clientSocket = clientSocket;
		}
		~errorContinueServ()
		{
			
		}
		std::string	getCodeError() const
		{
			return (_codeError);
		}
		t_configServ	getServ() const
		{
			return (_serv);
		}
		int	getClientSocket() const
		{
			return (_clientSocket);
		}
	private :
		std::string	_codeError;
		t_configServ	_serv;
		int	_clientSocket;
};

#include "../srcs/http/response/response.hpp"

int	checkTimeAndWaitPoll(t_server &data, std::vector<struct epoll_event> &events);
void	delEpollEvent(int &epollFd, int &socket);
void	addEpollEvent(int &epollFd, int &socket);
void	initAdresse(t_server &data);
int	initSocket(t_server &data);
void	acceptNewClient(t_server &data, int &serverSocket);
void	disconnectClient(t_server &data, int &socket);
void	addPlaceEventLog(int nfds, std::vector<struct epoll_event> &events);
void	delPlaceEventLog(int nfds, std::vector<struct epoll_event> &events);
void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader);
void	recvBase(t_server &data, int &clientSocket);
void	recvChunk(t_server &data, int &clientSocket);
int	is_chunked(char *buffer);
//void	execCgi(std::string &msg, int &clientSocket);
int	is_hexa(char * buffer);
void	manageClient(t_server &data, int &clientSocket);
int	parsingConf(t_server &data, char **argv, int argc);
int	cutEachServ(std::string &stringFile, std::vector<std::string> &eachServString);
int	fillServ(std::vector<t_configServ> &eachServ, std::vector<std::string> &eachServString);
void	getLocation(std::vector<t_location> &locationVec, std::string servString);
std::string	getStrInfo(std::string servString, std::string finding);
int	checkData(std::vector<t_configServ> &eachServ);
int	initAllServ(t_server &data);
t_configServ	findGoodServ(std::string &message, t_server &data, int clientSocket);
void	errorContinue(t_server &data, errorContinueServ const& e);
void	errorStop(t_server &data, errorStopServ const& e);
void	*ft_memset(void *dest, int c, size_t size);
void	chunkTest(t_server &data, int &clientSocket);
