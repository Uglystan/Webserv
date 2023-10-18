#include "../../include/serveur.hpp"

int	checkIp(t_configServ eachServ)
{
	unsigned int i = 0;
	unsigned int start = 0;
	int	groupeNbr = 0;
	int point = 0;
	int nbr = 0;

	if (eachServ.ip == "")
	{
		eachServ.ip = "0.0.0.0";
		return (0);
	}
	while (i < eachServ.ip.size())
	{
		nbr = 0;
		start = i;
		while(eachServ.ip[i] != '.' && i < eachServ.ip.size())
		{
			i++;
			nbr++;
		}
		groupeNbr++;
		if (atoi(eachServ.ip.substr(start, nbr).c_str()) > 255 || atoi(eachServ.ip.substr(start, nbr).c_str()) < 0)
			return (-1);
		if (eachServ.ip[i] == '.')
		{
			i++;
			point++;
		}
		if (nbr > 3 || nbr == 0)
			return (-1);
	}
	if (point != 3 || groupeNbr != 4)
		return (-1);
	return (0);
}

int	checkPort(t_configServ	eachServ)
{
	if (eachServ.port < 1024 || eachServ.port > 65534)
		return (-1);
	return (0);
}

int	checkData(std::vector<t_configServ> &eachServ)
{
	for (unsigned int i = 0; i < eachServ.size(); i++)
	{
		if (checkIp(eachServ[i]) == -1)
		{
			std::cerr << "Ip error" << std::endl;
			return (-1);
		}
		if (checkPort(eachServ[i]) == -1)
		{
			std::cerr << "Port error" << std::endl;
			return (-1);
		}
	}
	return (0);
}