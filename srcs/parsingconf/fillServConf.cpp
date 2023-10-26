#include "../../include/serveur.hpp"

std::string	getStrInfo(std::string servString, std::string finding)
{
	unsigned int	len = 0;
	unsigned int	start = servString.find(finding);
	if (servString.find(finding) == std::string::npos)
		return ("");
	start += finding.size();
	unsigned int i = start;
	for(; i < servString.size() && servString[i] > 32; i++)
		len++;
	if (i == servString.size() || len == 0)
		return ("");
	return(servString.substr(start, len));
}

std::string	getIp(std::string servString, int &port)
{
	std::string	ip;
	unsigned int	len = 0;
	unsigned int	start = servString.find("listen ");
	if (servString.find("listen ") == std::string::npos)
	{
		port = -1;
		return ("");
	}
	start += 7;
	unsigned int i = start;
	for(; i < servString.size() && servString[i] > 32; i++)
		len++;
	if (i == servString.size() || len == 0)
	{
		port = -1;
		return ("");
	}
	ip = servString.substr(start, len);
	if (ip.find(":") != std::string::npos)
	{
		unsigned int sizePort = ip.find(":") + 1;
		while (ip[sizePort] >= '0' && ip[sizePort] <= '9')
			sizePort++;
		port = atoi(ip.substr(ip.find(":") + 1, sizePort - ip.find(":")).c_str());
		return (ip.substr(0, ip.find(":")));
	}
	else
	{
		port = -1;
		return ("");
	}
}

int	getMaxBodySize(std::string servString)
{
	unsigned int	len = 0;
	unsigned int	start = servString.find("max_body_size ");
	if (servString.find("max_body_size ") == std::string::npos)
		return (0);
	start += 14;
	unsigned int i = start;
	for(; i < servString.size() && servString[i] > 32; i++)
		len++;
	if (i == servString.size() || len == 0)
		return (0);
	return(atoi(servString.substr(start, len).c_str()));
}

t_configServ	fillEachServ(std::string servString)
{
	t_configServ	serv;

	serv.serverName = getStrInfo(servString, "server_name ");
	serv.ip = getIp(servString, serv.port);
	serv.root = getStrInfo(servString, "root ");
	serv.index = getStrInfo(servString, "index ");
	serv.errorPage = getStrInfo(servString, "error_page ");
	serv.maxBodySize = getMaxBodySize(servString);
	serv.cgi = getStrInfo(servString, "cgi ");
	serv.cgiExt = getStrInfo(servString, "cgi_ext ");
	serv.allowMethods = getStrInfo(servString, "allow_methods ");
	serv.tmp = getStrInfo(servString, "tmp ");
	serv.listening_file = getStrInfo(servString, "listening_file ");
	getLocation(serv.locationVec, servString);
	serv.serverSocket = -1;
	return (serv);
}

int	fillServ(std::vector<t_configServ> &eachServ, std::vector<std::string> &eachServString)
{
	int	i = 0;
	while (i < (int)eachServString.size())
	{
		//std::cout << "Serveur : " << i << std::endl;
		eachServ.push_back(fillEachServ(eachServString[i]));
		i++;
	}
	return (0);
}