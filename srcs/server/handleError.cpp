#include "../../include/serveur.hpp"

void	errorContinue(t_server &data, errorContinueServ const& e)
{
	std::cout << e.getCodeError() << std::endl;
	Response reponse(e.getCodeError(), e.getServ());
	std::string response = reponse.statik_or_dynamik();
	if (send(e.getClientSocket(), response.data(), response.size(), 0) == -1)
		throw errorStopServ(strerror(errno));
	data.req.erase(e.getClientSocket());
	data.req[e.getClientSocket()].message.erase();
	close(e.getClientSocket());
}

void	errorStop(t_server &data, errorStopServ const& e)
{
	std::map<int, struct timeval>::iterator i = data.timer.begin();

	std::cout << e.getCodeError() << std::endl;
	for (; i != data.timer.end(); i++)
		close(i->first);
}