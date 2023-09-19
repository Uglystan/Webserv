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

#define NB_EVENT 10
#define MAX_CLIENT 100

std::string	get_method(std::string	request)
{
	std::string answer;
	size_t	position = request.find("GET /");
	position += 4;
	int i = 0;
	while (request[position + i] != ' ')
		i++;
	std::string path = request.substr(position, i);
	std::string htmlpath = "../html" + path + ".html";
	std::ifstream html_file;
	if (htmlpath == "../html/.html")
	{
		html_file.open("../html/monsite.html");
	}
	else
		html_file.open(htmlpath.c_str());
	if (html_file.is_open())
	{
		std::string buffer;
		while (std::getline(html_file, buffer))
		{
			if (!html_file.eof())
				answer.append(buffer + '\n');
			else //pas de \n si derniere ligne
				answer.append(buffer);
		}
		html_file.close();
	}
	else
	{
		 std::cerr << "Impossible d'ouvrir le fichier HTML." << std::endl;
	}
	//std::cout << answer << std::endl;
	return (answer);
}


std::string	parsing_request(std::string	request)
{
	size_t posGET = request.find("GET");
	size_t posPOST = request.find("POST");
	size_t posDELETE = request.find("DELETE");
	if (posGET != std::string::npos || posPOST != std::string::npos || posDELETE != std::string::npos)
	{
		if(posGET != std::string::npos)
			return (get_method(request));
		if(posPOST != std::string::npos)
			std::cout << "POST find" << std::endl;
		if(posDELETE != std::string::npos)
			std::cout << "DELETE find" << std::endl;
	}
	else if (request != "\n")
	{
		std::cout << "METHOD not found" << std::endl;
	}
	return (request);
}

void	initAdresse(struct sockaddr_in	&adresse)
{
	adresse.sin_family = AF_INET;
	adresse.sin_addr.s_addr = inet_addr("127.0.0.1");
	adresse.sin_port = htons(8080);
}

int	initSocket(struct sockaddr_in	&adresse)
{
	int	socketServeur;
	int opt = 1;
	//Creation socket client
	if ((socketServeur = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cout << "Echec de la creation socket" << std::endl;
		/*Gestion erreur*/
	}
	// Parametre de la socket on lui dis de pouvoir utiliser la meme adresse et d'ecouter sur le meme port donc on peut gerer plusieur requete
	if (setsockopt(socketServeur, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
	{
		std::cout << "Echec de la mise en place des options socket" << std::endl;
		/*Gestion erreur*/
	}
	//On place la socket sur le port et l'adresse IP defini
	if (bind(socketServeur, (struct sockaddr *) &adresse, sizeof(adresse)) != 0)
	{
		perror("Erreur bind");
		std::cout << "Echec de bind" << std::endl;
		/*Gestion erreur*/
	}
	//On fait ecouter la socket pour recevoir des requetes 2eme opt taille de file d'attente
	if (listen(socketServeur, 10) != 0)
	{
		std::cout << "Echec de listen" << std::endl;
		/*Gestion erreur*/
	}
	return (socketServeur);
}

//Fonction pour ajouter des socket aux evenement de Epool
void	addEpollEvent(int &epollFd, int &socket)
{
	//Ici la structure sert juste setup les evenements pour la socket et a l'ajouter a la liste d'evenement
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	//On gere Le in et si la socket se ferme
	event.events = EPOLLIN | EPOLLRDHUP;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, socket, &event);
}

int	waitForClient(int &serverSocket)
{
	int clientSocket;
	struct sockaddr_in clientAdresse;
	int addresseLen = sizeof(clientAdresse);
	clientSocket = accept(serverSocket, (struct sockaddr*) &clientAdresse, (socklen_t *) &addresseLen);
	if (clientSocket != -1)
	{
		//On met la socket en non bloquant c'est important sinon ca marche pas
		int opt = 1;
		setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(1));
	}
	return (clientSocket);
}

void	delEpollEvent(int &epollFd, int &socket)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(struct epoll_event));
	event.data.fd = socket;
	// On surveille les message entrant, la fermeture de la socket
	event.events = EPOLLIN | EPOLLRDHUP;
	epoll_ctl(epollFd, EPOLL_CTL_DEL, socket, &event);
}

void	manageClient(int &epollFd, int &clientSocket)
{
	char	buffer[1024 + 1];
	int	bytes_read = 0;
	int	socketStatement = 0;

	bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);
	if (bytes_read == -1)
	{
		std::cout << "Error recv" << std::endl;
		socketStatement = 1;
	}
	else if (bytes_read == 0)
	{
		std::cout << "Client deco" << std::endl;
		socketStatement = 1;
	}
	else if (bytes_read > 0)
	{
		std::cout << buffer << std::endl;
		std::string reponse = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\n\n<!DOCTYPE html><html><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html\n";
		send(clientSocket, reponse.c_str(), reponse.size(), 0);
	}
	if (socketStatement == 1)
	{
		delEpollEvent(epollFd, clientSocket);
		close(clientSocket);
	}
}

int main (/*File conf*/)
{
	int epollFd = epoll_create1(0);
	struct epoll_event	events[NB_EVENT];
	struct sockaddr_in	adresse;
	
	initAdresse(adresse);
	int serverSocket = initSocket(adresse);
	addEpollEvent(epollFd, serverSocket);
	int clientSocket;
	while(1)
	{
		//Le nombre de client qu'on souhaite gerer j'ai mis 2 au hasard. nfds c'est le nombre d'evenement qui on un truc EPOLLIN etc...
		int nfds = epoll_wait(epollFd, events, 2, -1);
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == serverSocket)//C'est la socket serveur donc on recoit un nouveau client
			{
				clientSocket = waitForClient(serverSocket);
				if (clientSocket != -1)
				{
					addEpollEvent(epollFd, clientSocket);
				}
			}
			else if (events[i].events & EPOLLIN)//C'est une socket client donc on doit gerer la demande
			{
				manageClient(epollFd, events[i].data.fd);
			}
		}
	}
	close(serverSocket);
}