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

#define NB_EVENT 10
#define MAX_CLIENT 100
#define MAX_CLIENT_BODYSIZE 20000
#define MAZ_TIME_BEFOR_CLOSE

int	checkTimeAndWaitPoll(int &epollFd, struct epoll_event *events, std::map<int, struct timeval> &timer);
void	delEpollEvent(int &epollFd, int &socket);
void	addEpollEvent(int &epollFd, int &socket);
void	initAdresse(struct sockaddr_in &adresse);
int	initSocket(struct sockaddr_in &adresse, int &epollFd);
void	acceptNewClient(int &serverSocket, std::map<int, struct timeval> &timer, int &epollFd);
void	manageClient(int &epollFd, int &clientSocket, std::map<int, struct timeval> &timer);
void	disconnectClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer);
void	errorClient(int &epollFd, int &socket, std::map<int, struct timeval> &timer);