/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourdon <abourdon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 11:42:04 by abourdon          #+#    #+#             */
/*   Updated: 2023/09/23 13:33:08 by abourdon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response(std::string request): _request(request), _code(200)
{
	cleanHeader();
	return ;
}

Response::~Response(void)
{
	return ;
}

void	Response::init_Error_code(void)
{
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[401] = "Unauthorized";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
}

void	Response::cleanHeader(void)
{
	_status_line = "";
	_server = "";
	_date = "";
	_content_type = "";
	_content_lenght = "";
	_connection = "";
	_ContentLanguage = "";
	_LastModified = "";
	_TransferEncoding = "";
	_WwwAuthenticate = "";
	_path = "";
	_body = "";
	_header = "";
}

std::string	Response::find_method(void)
{
	size_t posGET = _request.find("GET");
	size_t posPOST = _request.find("POST");
	size_t posDELETE = _request.find("DELETE");
	if (posGET != std::string::npos || posPOST != std::string::npos || posDELETE != std::string::npos)
	{
		if(posGET != std::string::npos)
			return (get_response());
		if(posPOST != std::string::npos)
			std::cout << "POST find" << std::endl;
		if(posDELETE != std::string::npos)
			std::cout << "DELETE find" << std::endl;
	}
	else if (_request != "\n")
	{
		//std::cout << "METHOD not found" << std::endl;
		_code = 400;
	}
	return (_response);
}

std::string	Response::get_response(void)
{
	find_path();
	create_body();
	if (_code >= 400)
		body_error_page();
	create_header();
	if (_code >= 400)
	{
		body_error_page();
		create_header();
	}
	_response = _header + _body;
	return (_response);
}

void	Response::create_body()
{
	std::ifstream html_file;
	if (_path == "html/")
		html_file.open("html/monsite.html", std::ios::in);
	else
		html_file.open(_path.c_str(), std::ios::in);
	if (html_file.is_open())
	{
		if (html_file.peek() ==  std::ifstream::traits_type::eof())
		{	
			_code = 204;
			return ;
		}
		std::string buffer;
		while (std::getline(html_file, buffer))
		{
			if (!html_file.eof())
				_body.append(buffer + '\n');
			else //pas de \n si derniere ligne
				_body.append(buffer);
		}
		html_file.close();
	}
	else
	{
		_code = 404;
		_path = "html/error.html";
		//create_body();//peu etre a laisser
		//std::cerr << "Impossible d'ouvrir le fichier HTML." << std::endl;
	}
}

void	Response::body_error_page(void)
{
	_body.clear();
	std::ifstream html_file("html/error.html");
	if (html_file.is_open())
	{
		if (html_file.peek() ==  std::ifstream::traits_type::eof())
			_code = 204;
		std::string buffer;
		while (std::getline(html_file, buffer))
		{
			if (!html_file.eof())
				_body.append(buffer + '\n');
			else //pas de \n si derniere ligne
				_body.append(buffer);
		}
		html_file.close();
	}
	else
	{
		_code = 404;
		//std::cerr << "Impossible d'ouvrir le fichier HTML." << std::endl
	}
	std::stringstream ss;
	ss << _code;
	size_t codePos = _body.find("[CODE]");
	if (codePos != std::string::npos) {
		_body.replace(codePos, 6, ss.str());
	}
	codePos = _body.find("[CODE]");
	if (codePos != std::string::npos) {
		_body.replace(codePos, 6, ss.str());
	}
	std::string errorMessage = _errors[_code];
	size_t messagePos = _body.find("[MESSAGE]");
	if (messagePos != std::string::npos) {
		_body.replace(messagePos, 9, errorMessage);
	}
	_code = 200;
}

void	Response::create_header(void)
{
	_header += find_status_line();
	_header += find_server();
	_header += find_date();
	_header += find_content_type();
	_header += find_content_lenght();
	_header += find_content_lang();
	_header += find_LastModified();
	_header += find_tranfertencoding();
	_header += find_WwwAuthenticate();
	_header += find_connection();
	_header += "\r\n";
}

std::string	Response::find_status_line(void)
{
	// find_error_code();
	init_Error_code();
	std::stringstream ss;
	ss << _code;
	_status_line = "HTTP/1.1 " + ss.str() + " " + _errors[_code] + "\n";
	return (_status_line);
}


std::string	Response::find_server(void)
{
	_server = "Server: webserver-42\n";
	return (_server);
}

std::string	Response::find_date(void)
{
	time_t		t_ptr;
	struct tm	*y2k;
	char buffer[80];

	std::time(&t_ptr);
	y2k = localtime(&t_ptr);
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", y2k);
	_date = "Date: " + std::string(buffer) + "\n";
	return (_date);
}

std::string	Response::find_content_type(void)
{
	if (find_langage() == "")
		_content_type = "Content-Type: text/html; charset=UTF-8\n";
	else if (find_langage() == "html" || find_langage() == "css" || find_langage() == "javascript" || find_langage() == "xml" || find_langage() == "plain")
		_content_type = "Content-Type: text/" + find_langage() +"; charset=UTF-8\n";
	else if (find_langage() == "jpeg" || find_langage() == "png" || find_langage() == "gif" || find_langage() == "bmp" || find_langage() == "ico")
		_content_type = "Content-Type: image/" + find_langage() +"; charset=UTF-8\n";
	else if (find_langage() == "mpeg" || find_langage() == "wav" || find_langage() == "ogg")
		_content_type = "Content-Type: audio/" + find_langage() +"; charset=UTF-8\n";
	else if (find_langage() == "mp4" || find_langage() == "webm" || find_langage() == "mpeg" || find_langage() == "avi")
		_content_type = "Content-Type: video/" + find_langage() +"; charset=UTF-8\n";
	else if (find_langage() == "json" || find_langage() == "pdf" || find_langage() == "octet-stream" || find_langage() == "x-www-form-urlencoded")
		_content_type = "Content-Type: application/" + find_langage() +"; charset=UTF-8\n";
	else if (find_langage() == "form-data" || find_langage() == "byteranges")
		_content_type = "Content-Type: multipart/" + find_langage() +"; charset=UTF-8\n";
	else
	{
		//_code = 400;
		_content_type = "Content-Type: text/html; charset=UTF-8\n";
	}
	return (_content_type);
}

std::string	Response::find_content_lenght(void)
{
	std::ostringstream oss;
	oss << "Content-Length: " << _body.size() << "\n";
	_content_lenght = oss.str();
	return (_content_lenght);
}

std::string	Response::find_connection(void)
{
	_connection = "Connection: keep-alive\n";
	return (_connection);
}

std::string	Response::find_content_lang(void)
{
	size_t startPos = _request.find("Accept-Language: ");
	_ContentLanguage = "Content-Language: ";
	if (startPos != std::string::npos)
	{
		startPos += sizeof("Accept-Language: ") - 1;
		size_t endPos = _request.find("\n", startPos);
		if (endPos != std::string::npos)
		{
			_ContentLanguage += _request.substr(startPos, endPos - startPos) + "\n";
		}
	}
	else
		_code = 400;
	return (_ContentLanguage);
}

std::string	Response::find_LastModified(void)
{
	struct tm	*y2k;//struct date heure
	char buffer[80];//stocker date
	struct	stat	stats;//stock info fichier
	if (stat(_path.c_str(), &stats) == 0)//obtient l'heure modification
	{
		y2k = gmtime(&stats.st_mtime);//permet d'avoir heure du fichier
		std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", y2k);
		_LastModified = "Last-Modified: " + std::string(buffer) + "\n";
	}
	return (_LastModified);
}

std::string	Response::find_tranfertencoding(void)
{
	_TransferEncoding = "Transfert-Encoding: identity\n";
	return (_TransferEncoding);
}

std::string	Response::find_WwwAuthenticate(void)
{
	if (_code == 401)
		_WwwAuthenticate = "WWW-Authenticate: Basic realm=\"Authentification access requires\"\n";
	return (_WwwAuthenticate);
}


void	Response::find_path(void)
{
	_path = "html/";
	std::string firstLine;
	std::istringstream iss(_request);
	std::getline(iss, firstLine);
	size_t	firstslahpos = firstLine.find('/');
	if (firstslahpos != std::string::npos)
	{
		size_t	nextspacepos = firstLine.find(' ', firstslahpos + 1);
		if (nextspacepos != std::string::npos)
			_path += firstLine.substr(firstslahpos + 1, nextspacepos - firstslahpos - 1);
	}
	else
		_code = 400;
}

std::string	Response::find_langage(void)
{
	std::string	language;
	std::string firstLine;
	std::istringstream iss(_request);
	std::getline(iss, firstLine);
	size_t	firstpointpos = firstLine.find('.');
	if (firstpointpos != std::string::npos)
	{
		size_t	nextspacepos = firstLine.find(' ', firstpointpos + 1);
		if (nextspacepos != std::string::npos)
			language += firstLine.substr(firstpointpos + 1, nextspacepos - firstpointpos - 1);
	}
	else
		_code = 400;
	return (language);
}



std::string	Response::get_response(void) const
{
	return (_response);
}