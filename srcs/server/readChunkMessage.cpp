#include "../../include/serveur.hpp"

int	is_chunked(char *buffer)
{
	std::string	buf = buffer;

	if (buf.find("Transfer-Encoding: chunked") != std::string::npos)
		return(1);
	else
		return (0);
}

int	is_hexa(char * buffer)
{
	for(int i = 0; buffer[i] != '\r'; i++)
	{
		if ((buffer[i] < '0' || buffer[i] > '9') || (buffer[i] < 'A' || buffer[i] > 'F') || (buffer[i] < 'a' || buffer[i] > 'f'))
			return (0);
	}
	return (1);
}

int	sizeTaille(char *buffer)
{
	int	i = 0;

	while(buffer[i] != '\r')
		i++;
	i += 2;
	return (i);
}

void	chunckedMessage(char *buffer, int &bytes_read, int &clientSocket, std::string &msg)
{
	int	totalBytesRead = 0;
	int	sizeMessageTotal;

	if (is_hexa(buffer) == 1)//hexa
	{
		sizeMessageTotal = sizeMessageChunk(buffer);
		std::string	sub = buffer;
		msg.append(sub.substr(sizeTaille(buffer), sizeMessageChunk(buffer)));
	}
	else//pas hexa
	{
		sizeMessageTotal = sizeHeader(buffer);
		msg.append(buffer);
	}
	totalBytesRead += bytes_read;
	while (totalBytesRead != sizeMessageTotal)//avant y'avait "<" et ca marchais !!!!!!!!!!!!!!!!!!!!!!
	{
		memset(buffer, 0, sizeof(&buffer));
		bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);//proteger recv encore ?
		msg.append(buffer);
		if (bytes_read != -1)
			totalBytesRead += bytes_read;
	}
	if (sizeMessageChunk(buffer) == 0)
	{
		//execCgi(msg, clientSocket);
		Response reponse(msg);
		std::string response = reponse.statik_or_dynamik();
		send(clientSocket, response.c_str(), response.size(), 0);
		msg.clear();
	}
}