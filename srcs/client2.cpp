#include <sys/socket.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <cstring>
#include<arpa/inet.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <cstdio>

std::vector<std::string> const	split(const std::string & ip, char delimiteur, std::vector<std::string> &elements)
{
	
	std::stringstream ss(ip);
	std::string sousChaine;
	while(getline(ss, sousChaine, delimiteur))
	{
		elements.push_back(sousChaine);
	}
	return (elements);
}

long	convert_ip(std::string ip)
{
	long	ip_long = 0;
	std::vector<std::string> ip_cut;
	split(ip, '.', ip_cut);
	for(int i = 0; i < 4; i++)
		ip_long = (atol(ip_cut[i].c_str()) * (pow(256, (3 - i)))) + ip_long;
	return (ip_long);
}

int main()
{
	/*-----------------VARIABLE-------------------*/
	//UNe socket ressemble a un fd c'est un int et il aura une valeur qui sera liee a une socket
	int sock;
	//ENsuite pour faire fonctionner la socket on doit utiliser la structure sockaddr_in qui va permettre de renseigner
	//le port, l'adresse ip, la famille de communication, ...
	struct sockaddr_in sock_adr;
	//Avec la fonction socket on initialise la socket on renseigne le domaine de communication en general AF_INET qui est
	//base sur l'IP. Ensuite on renseigne le type de socket donc ca defini si on utilise TCP ou UDP ici TCP
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//COmme pour les FD si il y'a une erreur sock sera = a -1
	if (sock < 0)
	{
		std::cout << "Error" << std::endl;
		return(1);
	}
	/*-----------------Initialisation des membre de l'objet serveur-------------------*/
	//On vient ensuite initialiser les elements de la structure sockaddr_in on met le domaine de communication dans
	//dans sin_family
	sock_adr.sin_family = AF_INET;
	//Dans sin_port on indique le numero du port aux quel on va se connecter pour un serveurweb generalement 80
	//Ici on utilise la fonction htons qui permet de convertir les type short, int, long quand on veut les envoyer a traver
	//le reseaux sans quoi ils seront corrompu cette fonctionne n'est pas necessaire si on travail avec le serveur et le client
	//sur le meme reseau mais il est recommande de le faire
	sock_adr.sin_port = htons(8080);
	//Ici on initialise l'adresse ip la fonction inet_addr sert a convertir une adresse ip en long
	sock_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*-----------------Connexion au serveur distant-------------------*/
	//Ici on connecte la socket avec la fonction connect qui prend la socket (sock), la structure
	//il faut par contre la caster en sockaddr * et la taille de la structure obtenue avec sizeof()
	//on controle si la fonction renvoie une valeure coherente avec le if
	if (connect(sock, (struct sockaddr *)&sock_adr, sizeof(sock_adr)) < 0)
	{
		std::cout << "Connect error" << std::endl;
		return (1);
	}
	std::cout << "Connected" << std::endl;
	/*-----------------Envoie de donnees-------------------*/
	std::string message = "GET / HTTP/1.1\n\n";
	//Ici on utilise la fonction send qui prend la socket, le message en char *, la taille du message
	//et des flags mais ici il n'y en aura pas toujours controle pour savoir si le message est envoye
	if (send(sock, message.c_str(), strlen(message.c_str()), 0) < 0)
	{
		std::cout << "Send failed" << std::endl;
		return (1);
	}
	std::cout << "Send success" << std::endl;
	/*-----------------Recevoir une reponse du serveur (Ici google)-------------------*/
	//Ici on utilise recv pour la reponse qui prend encore la socket et un buffer ou sera ecrite
	//la reponse ca peut etre bien de faire une boucle ou le buffer se remet a 0 en ayant mis ce
	//qu'il y'avait dedans dans un fichier
	char reponse[2000];
	if (recv(sock, reponse, 2000, 0) < 0)
	{
		std::cout << "recv failled" << std::endl;
		return (1);
	}
	std::cout << "reply received" << std::endl;
	printf("%s", reponse);
	close(sock);
	return (0);
}

//POur un serveur l'idee generale c'est qu'on a une boucle infini qui tourne avec la fonction listen
//la fonction va ecouter sur le port 80 logiquement qui attend que
//quelque chose se passe une fois qu'on recoit une socket on la lis et le decrypte et on repond la bonne
//chose donc soit on envoie de l'hmtl ou autre avec une socket on ne peut pas utiliser fork ou des thread
//Donc le serveur doit gerer de maniere sequentielle les requetes

/*En fait ce qui se passe c'est que tu demandes un socket à ton OS avec  `socket`,
mais pour l'instant c'est une coquille vide, ça ne sert à rien. Imagine une boite avec 4 slots,
deux slots pour une adresse et deux slots pour des ports.
Mainteant quand tu appelles `bind`, tu "remplis"  la moitié de la boîte avec une adresse et
un port qui correspondent à une addresse et un port de ta machine. Maintenant la socket est
lié à cette interface. Ensuite il faut activer cette sokcet avec `listen` pour qu'elle soit
capable d'"écouter", c'est à dire de recevoir des requetes sur cette interface*/

/**/