#include "response.hpp"

Response::Response(std::string request, t_configServ serv) : _code(200), _request(request), _serv(serv)
{
	cleanHeader();
}

Response::~Response(void)
{
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
	_path = "";
	_body = "";
	_header = "";
	_method = "";
	_status_line = "";
	_server_name = "";
	_date = "";
	_content_type = "";
	_content_lang = "";
	_content_lenght = "";
	_last_modified = "";
	_transfertencoding = "";
	_wwwauthenticate = "";
	_connection = "";
}

std::string	Response::statik_or_dynamik(void)
{
	init_Error_code();
	try
	{
		find_method();
		if (_serv.allowMethods.find(_method) == std::string::npos)
		{
			_code = 405;
			throw Response::Errorexcept();
		}
		_path = find_path(_request, _serv.root);
		if (_path == "")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
		size_t posPHP = _path.find(_serv.cgiExt);
		if (posPHP != std::string::npos)
			cgi_handler();
		else
			statik_response();
		std::cout << "Message envoyee : \n" << _response << std::endl;
		return (_response);
	}
	catch (const std::exception& e)
	{
		_response.clear();
		body_error_page();
		std::cout << "CODEEEEEEEEEEE : \n" << _code << std::endl;
		create_header();
		std::cerr << e.what() << std::endl;
		_response = _header + _body;
		std::cout << "Message envoyee : \n" << _response << std::endl;
		return (_response);
	}
}

void	Response::cgi_handler(void)
{
	put_in_env();
	std::string	postData = extractPostData(_request, _envcontent_type);
	std::cout << "POSTTTTTTTTTTTTTTTTTTTTTTTT\n" << postData << "LOL" << std::endl;
	if (_method == "POST")
	{
		if (postData == "")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
	}
	_body = execCgi(_path, postData, _envcontent_lenght, _serv.cgi);
	if (_body == "")
	{
		_code = 404;
		throw Response::Errorexcept();
	}
	create_header();
	_response = _header + _body;
}

void	Response::put_in_env(void)
{
	fill_strings();
	setenv("REQUEST_METHOD", _method.c_str(), 1);
	if (_method == "GET")
		setenv("QUERY_STRING", _query_string.c_str(), 1);
	if (_method == "POST")
	{
		setenv("CONTENT_TYPE", _envcontent_type.c_str(), 1);
		setenv("CONTENT_LENGTH", _envcontent_lenght.c_str(), 1);
	}
	setenv("REMOTE_ADDR", _remote_addr.c_str(), 1);
	setenv("SERVER_NAME", _server_name.c_str(), 1);
	setenv("SERVER_PORT", _server_port.c_str(), 1);
	setenv("SERVER_PROTOCOL", _server_protocol.c_str(), 1);
	setenv("REQUEST_URI", _path.c_str(), 1);
	setenv("REDIRECT_STATUS", "200", 1);
	setenv("DOCUMENT_ROOT", _document_root.c_str(), 1);
	setenv("SCRIPT_FILENAME", _script_filename.c_str(), 1);
	extern char** environ;
	for (char** env = environ; *env; ++env) {
		std::cout << *env << std::endl;
	}
}

void	Response::fill_strings(void)
{
	_document_root = "/html";//changer en fonction de config fill
	_script_filename = _path;
	_request_method = _method;
	if (_method == "GET")
	{
		_query_string = extractQueryString(_request);
		// if (_query_string == "-1")
		// {
		// 	_code = 400;
		// 	throw Response::Errorexcept();
		// }
	}
	if (_method == "POST")
	{
		_envcontent_type = extractContentType(_request);
		_envcontent_lenght = extractContentLength(_request);
		if (atoi(_envcontent_lenght.c_str()) > _serv.maxBodySize)
		{
			_code = 413;
			throw Response::Errorexcept();
		}
		// if (_envcontent_type == "" || _envcontent_lenght == "")
		// {
		// 	_code = 400;
		// 	throw Response::Errorexcept();
		// }
	}
	_remote_addr = _serv.ip;//changer en fonction de config fill
	_server_name = _serv.serverName;//changer en fonction de config fill
	std::stringstream ss;
	ss << _serv.port;
	_server_port = ss.str();//changer en fonction de config fill
	_server_protocol = "HTTP/1.1";//changer en fonction de config fill
	_request_uri = _path;
}

void	Response::find_method(void)//rajouter si code erreur lucas serveur
{
	size_t posGET = _request.find("GET");
	size_t posPOST = _request.find("POST");
	size_t posDELETE = _request.find("DELETE");
	if (posGET != std::string::npos || posPOST != std::string::npos || posDELETE != std::string::npos)
	{
		if(posGET != std::string::npos)
			_method = "GET";
		else if(posPOST != std::string::npos)
			_method = "POST";
		else
		{
			_code = 405;
			throw Response::Errorexcept();
		}
	}
}

void	Response::statik_response(void)
{
	create_body();
	create_header();
	_response = _header + _body;
}

void	Response::create_body()
{
	std::ifstream html_file;
	if (_method == "GET")
	{
		if (_path == _serv.root)
		{
			std::string index = _path + _serv.index;
			html_file.open(index.c_str(), std::ios::in);
		}
		else
			html_file.open(_path.c_str(), std::ios::in);
	}
	// else if(_method == "POST")
	// 	html_file.open("html/dowmload.html", std::ios::in);
	if (html_file.is_open())
	{
		if (html_file.peek() ==  std::ifstream::traits_type::eof())
		{
			_code = 404;
			throw Response::Errorexcept();
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
		throw Response::Errorexcept();
	}
}

void	Response::body_error_page(void)
{
	_body.clear();
	std::ifstream html_file(_serv.errorPage.c_str());
	if (html_file.is_open())
	{
		if (html_file.peek() ==  std::ifstream::traits_type::eof())
		{
			_code = 404;
			throw Response::Errorexcept();
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
		throw Response::Errorexcept();
	}
	std::stringstream ss;
	ss << _code;
	size_t codePos = _body.find("[CODE]");
	if (codePos != std::string::npos)
		_body.replace(codePos, 6, ss.str());
	codePos = _body.find("[CODE]");
	if (codePos != std::string::npos)
		_body.replace(codePos, 6, ss.str());
	std::string errorMessage = _errors[_code];
	size_t messagePos = _body.find("[MESSAGE]");
	if (messagePos != std::string::npos)
		_body.replace(messagePos, 9, errorMessage);
}

void	Response::create_header(void)
{
	_header.clear();
	_status_line = find_status_line();
	_name = find_server(_serv.serverName);
	_date = find_date();
	_content_type = find_content_type(_request) + "\n";
	_content_lenght = find_content_lenght(_body);
	_content_lang = find_content_lang(_request);
	_last_modified = find_LastModified(_path);
	_transfertencoding = find_tranfertencoding();
	_wwwauthenticate = find_WwwAuthenticate(_code);
	_connection = find_connection();
	if (_status_line == "" || _name == "" || _content_type == "" || _content_lenght == "")
	{
		_code = 400;
		throw Response::Errorexcept();
	}
	// if (_last_modified == "")
	// {
	// 	_code = 500;
	// 	throw Response::Errorexcept();
	// }
	_header += _status_line + _name + _date + _content_type + _content_lenght + _content_lang + _last_modified + _transfertencoding + _wwwauthenticate + _connection + "\r\n";
}

std::string	Response::find_status_line(void)
{
	std::stringstream ss;
	ss << _code;
	_status_line = "HTTP/1.1 " + ss.str() + " " + _errors[_code] + "\n";
	return (_status_line);
}

const char*	Response::Errorexcept::what(void) const throw()
{
	return ("ERROR PAGE\n");
}

std::string	Response::get_response(void) const
{
	return (_response);
}