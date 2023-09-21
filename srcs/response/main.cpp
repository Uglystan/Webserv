#include "response.hpp"
int	main()
{
	Response reponse("GET /monsite.html HTTP/1.1\nAccept-Language: fr\n");
	//Response reponse("GET /mob HTTP/1.1\nHost: 127.0.0.1:8080\nConnection: keep-alive\nsec-ch-ua: \"Brave\";v=\"117\", \"Not;A=Brand\";v=\"8\", \"Chromium\";v=\"117\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8\nSec-GPC: 1\nAccept-Language: fr\nSec-Fetch-Site: none\nSec-Fetch-Mode: navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: document\nAccept-Encoding: gzip, deflate, br\nIf-Modified-Since: Thu, 21 Sep 2023 13:31:32 GMT");

	reponse.create_response();
	std::cout << reponse.get_response() << std::endl;
}