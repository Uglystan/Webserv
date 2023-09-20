#include "response.hpp"
int	main()
{
	Response reponse("GET / HTTP/1.1\n");
	reponse.create_response();
	std::cout << reponse.get_response() << std::endl;
}