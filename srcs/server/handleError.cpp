#include "../../include/serveur.hpp"

/*Ici on a la gestion d'erreur une partie pour une erreure sur une socket par exemple aui n'entraine pas un arret du serveur
et une partie sur des fonction d'appels systeme qui entraine un arret complet du serveur*/

void	errorContinue(t_server &data, errorContinueServ const& e)
{
	std::cerr << e.getCodeError() << std::endl;
	Response reponse(e.getCodeError(), e.getServ());
	std::string response = reponse.statik_or_dynamik();
	int ret = send(e.getClientSocket(), response.data(), response.size(), 0);
	if (ret == -1 || ret == 0)
		throw errorStopServ(strerror(errno));
	data.req.erase(e.getClientSocket());
	data.req[e.getClientSocket()].message.erase();
	close(e.getClientSocket());
}

void	errorStop(t_server &data, errorStopServ const& e)
{
	std::map<int, struct timeval>::iterator i = data.timer.begin();

	std::cerr << e.getCodeError() << std::endl;
	for (; i != data.timer.end(); i++)
		close(i->first);
}