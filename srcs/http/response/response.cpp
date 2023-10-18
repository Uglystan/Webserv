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
	_errors[301] = "Moved Permanantely";
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
	_transfertencoding = "";
	_wwwauthenticate = "";
	_connection = "";
}

void	Response::check_location(void)
{
	if (_path != _serv.root)
	{
		std::string	pathwithoutroot = _path.substr(_serv.root.size(), _path.size() - _serv.root.size());
		for (size_t i = 0; i < _serv.locationVec.size(); ++i)
		{
			const t_location& location = _serv.locationVec[i];
			if (location.directory.find(pathwithoutroot) != std::string::npos)
			{
				if (location.allow_methods != "")
					_serv.allowMethods = location.allow_methods;
				if (location.root != "")
				{
					std::string newroot = location.root;
					_path = find_path(_request, newroot);
				}
				if (location.redirection != "")
				{
					if (location.root != "")
						_path = location.root + location.redirection;
					else
						_path = _serv.root + location.redirection;
					size_t	doubleslash = _path.find("//");
					_path.replace(doubleslash, 2, "/");
					_code = 301;
				}
			}
		}
	}
}

std::string	Response::statik_or_dynamik(void)
{
	init_Error_code();
	try
	{
		if (check_req_code(_request) != 0)
		{
			_code = check_req_code(_request);
			if (_code != 500)
			{
				create_header();
				return (_header);
			}
			else
				throw Response::Errorexcept();
		}
		find_method();
		if (_method == "DELETE")
		{
			return (_response);
		}
		_path = find_path(_request, _serv.root);
		if (_serv.root == "")
		{
			_code = 404;
			throw Response::Errorexcept();
		}
		if (_path == "")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
		if (list_dir()== 1)
			return (_response);
		check_location();
		if (_serv.allowMethods.find(_method) == std::string::npos)
		{
			_code = 405;
			throw Response::Errorexcept();
		}
		size_t extensioncgi = _serv.cgiExt.find(find_langage(_request));
		if (extensioncgi != std::string::npos)
		{
			if (_path == _serv.root)
				statik_response();
			else
				cgi_handler();
		}
		else
			statik_response();
		//std::cout << "Message envoyee : \n" << _response << std::endl;
		return (_response);
	}
	catch (const std::exception& e)
	{
		_response.clear();
		body_error_page();
		create_header();
		std::cerr << e.what() << std::endl;
		_response = _header + _body;
		//std::cout << "Message envoyee : \n" << _response << std::endl;
		return (_response);
	}
}

void	Response::cgi_handler(void)
{
	std::string	postData = "";
	if (_method == "POST")
	{
		std::string type = extractContentType(_request);
		postData = extractPostData(_request, type);
		if (postData == "")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
	}
	put_in_env(postData);
	std::string	goodpath = find_cgi_path(_serv.cgi, find_langage(_request));
	_body = execCgi(_path, postData, goodpath);
	if (_body == "")
	{
		_code = 500;
		throw Response::Errorexcept();
	}
	else if (_body == "-1")
	{
		_code = 404;
		throw Response::Errorexcept();
	}
	else if (_body == "-2")
	{
		_code = 401;
		throw Response::Errorexcept();
	}
	create_header();
	_response = _header + _body;
}

void	Response::put_in_env(std::string postData)
{
	DIR *repertoire;
	fill_strings(postData);
	setenv("REQUEST_METHOD", _method.c_str(), 1);
	if (_method == "GET")
	{
		setenv("QUERY_STRING", _query_string.c_str(), 1);
		unsetenv("CONTENT_TYPE");
		unsetenv("CONTENT_LENGTH");
	}
	if (_method == "POST")
	{
		unsetenv("QUERY_STRING");
		setenv("CONTENT_TYPE", _envcontent_type.c_str(), 1);
		setenv("CONTENT_LENGTH", _envcontent_lenght.c_str(), 1);
		std::string	tmproot = _serv.root +  _serv.tmp;
		repertoire = opendir(tmproot.c_str());
		if (repertoire == NULL)
		{
			_code = 500;
			throw Response::Errorexcept();
		}
		closedir(repertoire);
		setenv("STOCK_TMP", _serv.tmp.c_str(), 1);
	}
	setenv("REMOTE_ADDR", _remote_addr.c_str(), 1);
	setenv("SERVER_NAME", _server_name.c_str(), 1);
	setenv("SERVER_PORT", _server_port.c_str(), 1);
	setenv("SERVER_PROTOCOL", _server_protocol.c_str(), 1);
	setenv("REQUEST_URI", _path.c_str(), 1);
	setenv("REDIRECT_STATUS", "200", 1);
	setenv("DOCUMENT_ROOT", _document_root.c_str(), 1);
	setenv("SCRIPT_FILENAME", _script_filename.c_str(), 1);
	// extern char** environ;
	// for (char** env = environ; *env; ++env) {
	// 	std::cout << *env << std::endl;
	// }
}

void	Response::fill_strings(std::string postData)
{
	_document_root = _serv.root;
	_script_filename = _path;
	_request_method = _method;
	if (_method == "GET")
	{
		_query_string = extractQueryString(_request);
	}
	if (_method == "POST")
	{
		_envcontent_type = extractContentType(_request);
		_envcontent_lenght = extractContentLength(_request, postData);
		if (atoi(_envcontent_lenght.c_str()) > _serv.maxBodySize)
		{
			_code = 413;
			throw Response::Errorexcept();
		}
		if (_envcontent_type == "")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
	}
	_remote_addr = _serv.ip;
	_server_name = _serv.serverName;
	std::stringstream ss;
	ss << _serv.port;
	_server_port = ss.str();
	_server_protocol = "HTTP/1.1";
	_request_uri = _path;
}

void	Response::check_content_type(void)
{
	std::string language = "";
	size_t	filename = _request.find("filename=");
	if (filename != std::string::npos)
	{
		size_t extension = _request.find(".", filename);
		if (extension != std::string::npos)
		{
			size_t	nextquote = _request.find('\"', extension + 1);
			if (nextquote != std::string::npos)
				language += _request.substr(extension, nextquote - extension);
		}
	}
	if (extractContentType(_request).find("application/x-www-form-urlencoded") == std::string::npos)
	{
		if (language != ".jpg" && language != ".png" && language != ".gif"
		&& language != ".bmp" && language != ".ico" && language != ".gif" && language != ".mpeg" 
		&& language != ".wav" && language != ".ogg" && language != ".mp4" && language != ".avi" 
		&& language != ".webm" && language != ".json" && language != ".pdf" && language != ".xml"
		&& language != ".txt" && language != ".mp3")
		{
			_code = 400;
			throw Response::Errorexcept();
		}
	}
}

void	Response::delete_method(void)
{
	_path = find_path(_request, _serv.root);
	if (_path.find("tmp/") == std::string::npos)
	{
		_code = 401;
		throw Response::Errorexcept();
	}
	_method = "DELETE";
	if (_serv.allowMethods.find(_method) == std::string::npos)
	{
		_code = 405;
		throw Response::Errorexcept();
	}
	if (delete_file(_path) == 0)
	{
		_code = 204;
		_path = _serv.root + _serv.index;
		create_body();
		create_header();
		_response = _header + _body;
	}
	else
	{
		_code = 401;
		throw Response::Errorexcept();
	}
}

void	Response::find_method(void)
{
	size_t posGET = _request.find("GET");
	size_t posPOST = _request.find("POST");
	size_t posDELETE = _request.find("DELETE");
	if(posGET != std::string::npos)
		_method = "GET";
	else if(posPOST != std::string::npos)
	{
		_code = 201;
		_method = "POST";
		check_content_type();
	}
	else if (posDELETE != std::string::npos)
		delete_method();
	else
	{
		_code = 405;
		throw Response::Errorexcept();
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
	if (_method == "GET" || _method == "DELETE")
	{
		if (_path == _serv.root)
		{
			std::string index = _path + _serv.index;
			html_file.open(index.c_str(), std::ios::in);
		}
		else
			html_file.open(_path.c_str(), std::ios::in);
	}
	if (html_file.is_open())
	{
		std::string buffer;
		std::getline(html_file, buffer);
		if (buffer.find("#!/") != std::string::npos && buffer.find("/bin/") != std::string::npos)
		{
			_code = 500;
			throw Response::Errorexcept();
		}
		_body.append(buffer + '\n');
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
		_body = "<!DOCTYPE html><html><h1>wrong error page path in config file</h1></html>";
		return ;
	}
	if (_body.find("[CODE]") != std::string::npos && _body.find("[MESSAGE]") != std::string::npos)
	{
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
}

void	Response::create_header(void)
{
	_header.clear();
	_status_line = find_status_line();
	_name = find_server(_serv.serverName);
	_date = find_date();
	_content_type = find_content_type(_request, _method) + "\n";
	_content_lenght = find_content_lenght(_body);
	_content_lang = find_content_lang(_request);
	_last_modified = find_LastModified(_path);
	_transfertencoding = find_tranfertencoding();
	_wwwauthenticate = find_WwwAuthenticate(_code);
	_connection = find_connection();
	if (_status_line == "" || _name == "" || _content_type == "")
	{
		_code = 400;
		throw Response::Errorexcept();
	}
	_header += _status_line + _name + _date + _content_type + _content_lenght + _content_lang + _last_modified + _transfertencoding + _wwwauthenticate + _connection + "\r\n";
}

int	Response::list_dir(void)
{
	DIR *repertoire;
	std::string vide = "";
	repertoire = opendir(find_path(_request, vide).c_str());
	if (repertoire != NULL)
	{
		struct dirent *entree;
		if (_serv.listening_file == "on")
		{
			std::string withoutroot = find_path(_request, vide);
			if (withoutroot.find(_serv.root) == std::string::npos && withoutroot.find(_serv.root.substr(0, _serv.root.size() - 1)) == std::string::npos)
			{
				_code = 401;
				throw Response::Errorexcept();
			}
			_body =  "<!DOCTYPE html><html><head><title>Repertory listing</title></head><body>";
			_body += "<h1>Repertory listing</h1>";
			_body += "<ul>";
			while ((entree = readdir(repertoire)) != NULL)
			{
				std::string filename = entree->d_name;
				_body += repo_listing(_path, filename, _serv.root, _request, _serv.ip, _serv.port);
				// std::string name = entree->d_name;
				// std::stringstream ss;
				// std::stringstream ss2;
				// ss << _serv.ip;
				// ss2 << _serv.port;
				// if (filename == ".")
				// {
				// 	filename = "http://" + ss.str() + ":" + ss2.str() + '/' + _path;
				// }
				// else if (filename == "..")
				// {
				// 	filename = "http://" + ss.str() + ":" + ss2.str() + '/' + _path;
				// 	if (_path[_path.size()- 1] != '/')
				// 	{
				// 		size_t POSslash = filename.rfind("/");
				// 		filename = filename.substr(0, POSslash);
				// 	}
				// 	else
				// 	{
				// 		size_t POSslash = filename.rfind("/");
				// 		size_t POS2slash = filename.rfind("/", POSslash - 1);
				// 		filename = filename.substr(0, POS2slash);;
				// 	}
				// }
				// else
				// {
				// 	if (find_path(_request, vide).find(_serv.root) == std::string::npos)
				// 		filename = "http://" + ss.str() + ":" + ss2.str() + '/' + _serv.root + filename;
				// 	else
				// 	{
				// 		if (_path[_path.size()- 1] != '/')
				// 			filename = "http://" + ss.str() + ":" + ss2.str() + '/' + _path + '/' + filename;
				// 		else
				// 			filename = "http://" + ss.str() + ":" + ss2.str() + '/' + _path + filename;
				// 	}
				// }
				// _body += "<li><a href='" + filename + "'>" + name + "</a></li>";
			}
			_body += "</ul>";
			_body += "</body></html>";
			create_header();
			_response = _header + _body;
			closedir(repertoire);
			return (1);
		}
		else
			_path = _serv.root + _serv.index;
	}
	return (0);
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
	return ("");
}

std::string	Response::get_response(void) const
{
	return (_response);
}