#include "../../include/serveur.hpp"

void	recvMessage(int &bytes_read, int &clientSocket, char *buffer)
{
	std::string	msg;
	int	totalBytesRead = 0;
	int	sizeMessageTotal = sizeMessage(buffer);

	msg.append(buffer);
	totalBytesRead += bytes_read;
	while (totalBytesRead != sizeMessageTotal)//avant y'avait "<" et ca marchais !!!!!!!!!!!!!!!!!!!!!!
	{
		memset(buffer, 0, sizeof(&buffer));
		bytes_read = recv(clientSocket, buffer, 1024, MSG_DONTWAIT);//proteger recv encore ?
		msg.append(buffer);
		if (bytes_read != -1)
			totalBytesRead += bytes_read;
	}
	std::cout << "Message recu : " << msg << std::endl;
	Response reponse(msg);
	std::string response = reponse.statik_or_dynamik();
	//std::cout << "Message envoye : " << response << std::endl;
	send(clientSocket, response.c_str(), response.size(), 0);
	//execCgi(msg, clientSocket);
}