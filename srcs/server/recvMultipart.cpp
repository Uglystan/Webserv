#include "../../include/serveur.hpp"

void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader)
{
	size_t sizeAll = 0;

	sizeAll = atoi(data.req[clientSocket].message.substr(data.req[clientSocket].message.find("Content-Length: ") + 16, 10).c_str()) + sizeHeader + 4;
	if (data.req[clientSocket].bytes >= (int)sizeAll)
	{
		std::cout << "Message recu en plusieur part : " << data.req[clientSocket].message << std::endl;
		std::cout << "STOP" << std::endl;
		Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data));//alex doit regarder maxdodysize
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
	std::cout << "STOP" << std::endl;
	Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data));
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
		//std::cout << "Message recu en chunk : " << data.req[clientSocket].message << std::endl;
		chunkTest(data, clientSocket);
		Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data));
		std::string response = reponse.statik_or_dynamik();
		if (send(clientSocket, response.data(), response.size(), 0) == -1)
			throw errorStopServ(strerror(errno));
		data.req.erase(clientSocket);
		data.req[clientSocket].message.erase();
	}
}