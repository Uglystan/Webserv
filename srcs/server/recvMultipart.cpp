#include "../../include/serveur.hpp"

/*On gere la reception des different format de message (Chunked, en une partie, en plusieur partie)
En une partie si on a pas de content lenght et pas de transfert encoding : chunked on attend de recevoir le message et on le traite et le renvoie
EN plusieur partie On va continuer a lire tant que le content lenght du body + du header et different a la taille du message ensuite on traite et on envoie
En Chunked on continue de lire et quand on recoit le flag de fin on traite et on envoie la requete
(Pour chaque message on a une map avec le num de socket ssocie a une structure avec les message et la taille actuel recu)*/

void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader)
{
	size_t sizeAll = 0;

	sizeAll = atoi(data.req[clientSocket].message.substr(data.req[clientSocket].message.find("Content-Length: ") + 16, 10).c_str()) + sizeHeader + 4;
	if (data.req[clientSocket].bytes >= (int)sizeAll)
	{
		std::cout << "Message recu en plusieur part : " << data.req[clientSocket].message << std::endl;
		Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data, clientSocket));//alex doit regarder maxdodysize
		std::string response = reponse.statik_or_dynamik();
		if (send(clientSocket, response.data(), response.size(), 0) == -1)
			throw errorStopServ(strerror(errno));
		data.req.erase(clientSocket);//suppr les save de bytes car message envoyee
		data.req[clientSocket].message.erase();//suppr le message car message envoyee
	}
}

void	recvBase(t_server &data, int &clientSocket)
{
	std::cout << "Message recu en une partie : " << data.req[clientSocket].message << std::endl;
	Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data, clientSocket));
	std::string response = reponse.statik_or_dynamik();
	if (send(clientSocket, response.data(), response.size(), 0) == -1)
		throw errorStopServ(strerror(errno));
	data.req.erase(clientSocket);
	data.req[clientSocket].message.erase();
}

void	recvChunk(t_server &data, int &clientSocket)
{
	if (data.req[clientSocket].message.find("0\r\n\r\n") != std::string::npos)//message contient encore les tailles
	{
		std::cout << "Message recu en chunk : " << data.req[clientSocket].message << std::endl;
		chunkTest(data, clientSocket);
		Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data, clientSocket));
		std::string response = reponse.statik_or_dynamik();
		if (send(clientSocket, response.data(), response.size(), 0) == -1)
			throw errorStopServ(strerror(errno));
		data.req.erase(clientSocket);
		data.req[clientSocket].message.erase();
	}
}