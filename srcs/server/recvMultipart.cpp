#include "../../include/serveur.hpp"

void	recvMulti(t_server &data, int &clientSocket, size_t &sizeHeader)
{
	size_t sizeAll = 0;

	sizeAll = atoi(data.req[clientSocket].message.substr(data.req[clientSocket].message.find("Content-Length: ") + 16, 10).c_str()) + sizeHeader + 4;
	if (data.req[clientSocket].bytes >= (int)sizeAll)
	{
		std::cout << "Message recu en plusieur part : " << data.req[clientSocket].message << std::endl;
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
	Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data));
	std::string response = reponse.statik_or_dynamik();
	if (send(clientSocket, response.data(), response.size(), 0) == -1)
		throw errorStopServ(strerror(errno));
	data.req.erase(clientSocket);
	data.req[clientSocket].message.erase();
}

int	is_hexa(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		return (0);
	return (1);
}

int	next_line(t_server &data, int i, int clientSocket)
{
	while(data.req[clientSocket].message[i] != '\n')
		i++;
	return (i + 1);
}

void	chunkTest(t_server &data, int &clientSocket)
{
	int	j;
	int i = 0;
	while (i <= data.req[clientSocket].bytes)
	{
		j = i;
		while (is_hexa(data.req[clientSocket].message[j]) == 0)
		{
			j++;
			if (data.req[clientSocket].message[j] == '\r')
			{
				data.req[clientSocket].message.erase(i, j + 1);
				break;
			}
		}
		i = next_line(data, i, clientSocket);
	}
}

void	recvChunk(t_server &data, int &clientSocket)
{
	if (data.req[clientSocket].message.find("0\r\n\r\n") != std::string::npos)//message contient encore les tailles
	{
		std::cout << "Message recu en chunk : " << data.req[clientSocket].message << std::endl;
		chunkTest(data, clientSocket);
		Response reponse(data.req[clientSocket].message, findGoodServ(data.req[clientSocket].message, data));
		std::string response = reponse.statik_or_dynamik();
		if (send(clientSocket, response.data(), response.size(), 0) == -1)
			throw errorStopServ(strerror(errno));
		data.req.erase(clientSocket);
		data.req[clientSocket].message.erase();
	}
}