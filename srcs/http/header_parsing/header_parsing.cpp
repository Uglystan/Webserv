#include "header_parsing.hpp"

std::string	find_server(std::string servname)
{
	std::string server = "Server: " + servname + "\n";
	return (server);
}

std::string	find_date(void)
{
	time_t		t_ptr;
	struct tm	*y2k;
	char buffer[80];

	std::time(&t_ptr);
	y2k = localtime(&t_ptr);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", y2k);
	std::string date = "Date: " + std::string(buffer) + "\n";
	return (date);
}

std::string	check_interomark(std::string language)
{
	size_t	interomark = language.find("?");
	if (interomark != std::string::npos)
		return (language.substr(0, interomark));
	else
		return (language);
}

std::string	find_langage(std::string &request)
{
	std::string	language;
	std::string firstLine;
	std::istringstream iss(request);
	std::getline(iss, firstLine);
	size_t	firstpointpos = firstLine.find('.');
	if (firstpointpos != std::string::npos)
	{
		size_t	nextspacepos = firstLine.find(' ', firstpointpos + 1);
		if (nextspacepos != std::string::npos)
			language += firstLine.substr(firstpointpos + 1, nextspacepos - firstpointpos - 1);
	}
	else
		return ("");
	return (check_interomark(language));
}

std::string	find_content_type(std::string &request)
{
	std::string content_type;
	if (find_langage(request) == "" || find_langage(request) == "php")
		content_type = "Content-Type: text/html; charset=UTF-8";
	else if (find_langage(request) == "html" || find_langage(request) == "css" || find_langage(request) == "javascript" || find_langage(request) == "xml" || find_langage(request) == "plain")
		content_type = "Content-Type: text/" + find_langage(request) +"; charset=UTF-8";
	else if (find_langage(request) == "jpg" || find_langage(request) == "png" || find_langage(request) == "gif" || find_langage(request) == "bmp" || find_langage(request) == "ico")
		content_type = "Content-Type: image/" + find_langage(request) +"; charset=UTF-8";
	else if (find_langage(request) == "mpeg" || find_langage(request) == "wav" || find_langage(request) == "ogg")
		content_type = "Content-Type: audio/" + find_langage(request) +"; charset=UTF-8";
	else if (find_langage(request) == "mp4" || find_langage(request) == "webm" || find_langage(request) == "mpeg" || find_langage(request) == "avi")
		content_type = "Content-Type: video/" + find_langage(request) +"; charset=UTF-8";
	else if (find_langage(request) == "json" || find_langage(request) == "pdf" || find_langage(request) == "octet-stream" || find_langage(request) == "x-www-form-urlencoded")
		content_type = "Content-Type: application/" + find_langage(request) +"; charset=UTF-8";
	else if (find_langage(request) == "form-data" || find_langage(request) == "byteranges")
		content_type = "Content-Type: multipart/" + find_langage(request) +"; charset=UTF-8";
	else
		content_type = "Content-Type: text/html; charset=UTF-8";
	return (content_type);
}

std::string	find_content_lenght(std::string &body)
{
	std::ostringstream oss;
	if (body.empty())
		return ("");
	oss << "Content-Length: " << body.size() << "\n";
	std::string content_lenght = oss.str();
	return (content_lenght);
}

std::string	find_connection(void)
{
	return ("Connection: keep-alive\n");
}

std::string	find_content_lang(std::string &request)
{
	size_t startPos = request.find("Accept-Language: ");
	std::string ContentLanguage = "Content-Language: ";
	if (startPos != std::string::npos)
	{
		startPos += sizeof("Accept-Language: ") - 1;
		size_t endPos = request.find("\n", startPos);
		if (endPos != std::string::npos)
		{
			ContentLanguage += request.substr(startPos, endPos - startPos) + "\n";
		}
	}
	else
		return ("");
	return (ContentLanguage);
}

std::string	find_LastModified(std::string &path)
{
	std::string	lastModified;
	struct tm	*y2k;//struct date heure
	char buffer[80];//stocker date
	struct	stat	stats;//stock info fichier
	if (stat(path.c_str(), &stats) == 0)//obtient l'heure modification
	{
		y2k = gmtime(&stats.st_mtime);//permet d'avoir heure du fichier
		std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", y2k);
		lastModified = "Last-Modified: " + std::string(buffer) + "\n";
	}
	else
		return ("");
	return (lastModified);
}

std::string	find_tranfertencoding(void)
{
	return ("Transfert-Encoding: identity\n");
}

std::string	find_WwwAuthenticate(int code)
{
	if (code == 401)
		return("WWW-Authenticate: Basic realm=\"Authentification access requires\"\n");
	return ("");
}

std::string find_path(std::string &request, std::string &root)
{
	std::string	path = root;
	std::string firstLine;
	std::istringstream iss(request);
	std::getline(iss, firstLine);
	size_t firstSlashPos = firstLine.find('/');
	if (firstSlashPos != std::string::npos)
	{
		size_t nextSpacePos = firstLine.find(" HTTP", firstSlashPos + 1);
		if (nextSpacePos != std::string::npos)
		{
			size_t nextQuestionMark = firstLine.find('?', firstSlashPos);
			if (nextQuestionMark != std::string::npos && nextQuestionMark < nextSpacePos)
			{
				// Si la chaîne de requête existe avant le prochain espace, stockez le chemin jusqu'au '?'
				path += firstLine.substr(firstSlashPos + 1, nextQuestionMark - firstSlashPos - 1);
			}
			else
			{
				// Si la chaîne de requête n'existe pas ou après le prochain espace, stockez le chemin jusqu'à l'espace
				path += firstLine.substr(firstSlashPos + 1, nextSpacePos - firstSlashPos - 1);
			}
		}
	}
	else
		return ("");
	std::cout << path << std::endl;
	return (path);
}

int	check_req_code(std::string request)
{
	int code = atoi(request.c_str());
	if (code != 0)
		return (code);
	return (0);
}