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
#include "../srcs/utils/gnl.hpp"
#include <cstdlib>
#include "../srcs/response/response.hpp"

#define NB_EVENT_BASE 100
#define MAX_CLIENT_BODYSIZE 100000
#define MAX_TIME_BEFORE_CLOSE 300

typedef struct s_server
{
	int epollFd;
	int serverSocket;
	struct sockaddr_in	adresse;
	std::map <int, struct timeval>	timer;
	std::map <int, std::string>	request;
	int	bytes_read;

}t_server;

int	checkTimeAndWaitPoll(t_server &data, std::vector<struct epoll_event> &events);
void	delEpollEvent(int &epollFd, int &socket);
void	addEpollEvent(int &epollFd, int &socket);
void	initAdresse(t_server &data);
int	initSocket(t_server &data);
void	acceptNewClient(t_server &data);
void	disconnectClient(t_server &data, int &socket);
void	errorClient(t_server &data, int &socket);
void	addPlaceEventLog(int nfds, std::vector<struct epoll_event> &events);
void	delPlaceEventLog(int nfds, std::vector<struct epoll_event> &events);
void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader);
void	recvBase(t_server &data, int &clientSocket);
void	recvChunk(t_server &data, int &clientSocket);
int	is_chunked(char *buffer);
void	execCgi(std::string &msg, int &clientSocket);
int	is_hexa(char * buffer);
void	manageClient(t_server &data, int &clientSocket);