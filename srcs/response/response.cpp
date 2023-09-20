/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgirault <lgirault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 11:42:04 by abourdon          #+#    #+#             */
/*   Updated: 2023/09/20 15:30:15 by lgirault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response(std::string request): _request(request)
{
	return ;
}

Response::Response(const Response &copy)
{
	*this = copy;
	return ;
}

Response	&Response::operator=(const Response &substitue)
{
	if (this != &substitue)
	{
		_response = substitue._response;
		_request = substitue._request;
		_status_line = substitue._status_line;
		_server = substitue._server;
		_date = substitue._date;
		_content_type= substitue._content_type;
		_content_lenght = substitue._content_lenght;
		_connection = substitue._connection;
	}
	return (*this);
}

Response::~Response(void)
{
	return ;
}

std::string	Response::find_status_line(void)
{
	_status_line = "HTTP/1.1 200 OK\n";
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
	_content_type = "Content-Type: text/html; charset=UTF-8\n";
	return (_content_type);
}

std::string	Response::find_content_lenght(void)
{
	_content_lenght = "Content-Lenght: 88\n";
	return (_content_lenght);
}

std::string	Response::find_connection(void)
{
	_connection = "Connection: Closed\n";
	return (_connection);
}

void	Response::find_method(void)
{
	size_t posGET = _request.find("GET");
	size_t posPOST = _request.find("POST");
	size_t posDELETE = _request.find("DELETE");
	if (posGET != std::string::npos || posPOST != std::string::npos || posDELETE != std::string::npos)
	{
		if(posGET != std::string::npos)
			create_response();
		if(posPOST != std::string::npos)
			std::cout << "POST find" << std::endl;
		if(posDELETE != std::string::npos)
			std::cout << "DELETE find" << std::endl;
	}
	else if (_request != "\n")
	{
		std::cout << "METHOD not found" << std::endl;
	}
}

void	Response::find_path(void)
{
	_path = "../../html/";
	std::string firstLine;
	std::istringstream iss(_request);
	std::getline(iss, firstLine);
	size_t	firstslahpos = firstLine.find('/');
	if (firstslahpos != std::string::npos)
	{
		size_t	nextspacepos = firstLine.find(' ', firstslahpos + 1);
		if (nextspacepos != std::string::npos)
			_path += firstLine.substr(firstslahpos + 1, nextspacepos - firstslahpos);
	}
}

std::string	Response::create_response(void)
{
	find_path();
	create_body();
	create_header();
	_response = _header + _body;
	return (_response);
	/*body html & assembler*/
}

void	Response::create_body()
{
	std::ifstream html_file;
	if (_path == "../../html/ ")
	{
		html_file.open("html/monsite.html", std::ios::in);
	}
	else
		std::cout << "find langage" << std::endl;//trouver .html ou .css....
	if (html_file.is_open())
	{
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
		 std::cerr << "Impossible d'ouvrir le fichier HTML." << std::endl;
	}
}

void	Response::create_header(void)
{
	_header += find_status_line();
	_header += find_server();
	_header += find_date();
	_header += find_content_type();
	_header += find_content_lenght();
	_header += find_connection();
	_header += "\n";
}

std::string	Response::get_response(void) const
{
	return (_response);
}

std::string	Response::get_request(void) const
{
	return (_request);
}

std::string	Response::get_status_line(void) const
{
	return (_status_line);
}

std::string	Response::get_server(void) const
{
	return (_server);
}

std::string	Response::get_date(void) const
{
	return (_date);
}

std::string	Response::get_content_type(void) const
{
	return (_content_type);
}

std::string	Response::get_content_lenght(void) const
{
	return (_content_lenght);
}

std::string	Response::get_connection(void) const
{
	return (_connection);
}

void	Response::set_response(std::string response)
{
	_response = response;
}
void	Response::set_request(std::string request)
{
	_request = request;
}

void	Response::set_status_line(std::string status_line)
{
	_status_line = status_line;
}

void	Response::set_server(std::string server)
{
	_server = server;
}

void	Response::set_date(std::string date)
{
	_date = date;
}

void	Response::set_content_type(std::string content_type)
{
	_content_type = content_type;
}

void	Response::set_content_lenght(std::string content_lenght)
{
	_content_lenght = content_lenght;
}

void	Response::set_connection(std::string connection)
{
	_connection = connection;
}