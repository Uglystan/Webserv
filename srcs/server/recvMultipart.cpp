#include "../../include/serveur.hpp"

void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader)
{
	size_t sizeAll = 0;

	sizeAll = atoi(data.request[clientSocket].substr(data.request[clientSocket].find("Content-Length: ") + 16, 10).c_str()) + sizeHeader + 4;
	if (data.bytes[clientSocket] >= (int)sizeAll)
	{
		std::cout << "Message recu en plusieur part : " << data.request[clientSocket] << std::endl;
		Response reponse(data.request[clientSocket]);
		std::string response = reponse.statik_or_dynamik();
		send(clientSocket, response.c_str(), response.size(), 0);
		data.bytes.erase(clientSocket);//suppr les save de bytes car message envoyee
		data.request[clientSocket].erase();//suppr le message car message envoyee
	}
}

void	recvBase(t_server &data, int &clientSocket)
{
	std::cout << "Message recu en une partie : " << data.request[clientSocket] << std::endl;
	Response reponse(data.request[clientSocket]);
	std::string response = reponse.statik_or_dynamik();
	send(clientSocket, response.c_str(), response.size(), 0);
	data.bytes.erase(clientSocket);
	data.request[clientSocket].erase();
}

void	recvChunk(t_server &data, int &clientSocket)
{
	if (data.request[clientSocket].find("0\r\n\r\n") != std::string::npos)
	{
		std::cout << "Message recu en chunk : " << data.request[clientSocket] << std::endl;
		Response reponse(data.request[clientSocket]);
		std::string response = reponse.statik_or_dynamik();
		send(clientSocket, response.c_str(), response.size(), 0);
		data.bytes.erase(clientSocket);
		data.request[clientSocket].erase();
	}
}