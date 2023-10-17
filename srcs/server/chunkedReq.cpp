#include "../../include/serveur.hpp"

/*Groupe de fonction utilse pour supprimer dans les request chunked les taille en hexadecimal et pouvoir assembler le message
correctement*/

int	is_hexa(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
		return (0);
	return (1);
}

int	next_line(t_server &data, int i, int clientSocket)
{
	while(data.req[clientSocket].message[i] != '\n' && i <= data.req[clientSocket].bytes)
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
				data.req[clientSocket].message.erase(i, j + 2 - i);
				break;
			}
		}
		i = next_line(data, i, clientSocket);
	}
}