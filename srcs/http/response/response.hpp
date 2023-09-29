/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourdon <abourdon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:26:10 by abourdon          #+#    #+#             */
/*   Updated: 2023/09/29 17:51:57 by abourdon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <sstream>
#include <cstdlib>
#include "../cgi_handler/cgi_handler.hpp"
#include "../CGIparsing/CGIparsing.hpp"

class Response
{
	public:
		Response(std::string request);
		~Response(void);

		std::string GetFileSizeAsString(void);
		void	statik_response(void);
		std::string	post_response(void);
		void	find_method(void);
		std::string	statik_or_dynamik(void);
		void	find_path(void);
		void	create_header(void);
		void	create_body();
		void	init_Error_code(void);
		void	find_error_code(void);
		void	body_error_page(void);
		void	cleanHeader(void);
		void	cgi_handler(void);
		void	put_in_env(void);
		void	fill_strings(void);
		std::string	find_status_line(void);
		std::string	find_server(void);
		std::string	find_date(void);
		std::string	find_content_type(void);
		std::string	find_content_lenght(void);
		std::string	find_connection(void);
		std::string	find_langage(void);
		std::string	find_content_lang(void);
		std::string	find_LastModified(void);
		std::string	find_tranfertencoding(void);
		std::string	find_WwwAuthenticate(void);
		std::string	get_response(void) const;

	private:
		std::string	_response;
		std::string	_request;
		std::string	_status_line;
		std::string	_server;
		std::string	_date;
		std::string	_content_type;
		std::string	_content_lenght;
		std::string	_connection;
		std::string	_ContentLanguage;
		std::string	_LastModified;
		std::string	_TransferEncoding;
		std::string	_WwwAuthenticate;
		std::string	_path;
		std::string	_body;
		std::string	_header;
		std::string	_method;
		int		_code;
		int		_bool;
		std::map<int, std::string>	_errors;

		std::string	_document_root;//repertoire racine
		std::string	_script_filename;//script part
		std::string	_query_string;//parametres de GET
		std::string	_request_method;//method
		std::string	_envcontent_type;//type pour POST
		std::string	_envcontent_lenght;//taille contenu pour post
		std::string	_remote_addr;//ip cient de la requete
		std::string	_server_name;//server name
		std::string	_server_port;//port server listen
		std::string	_server_protocol;//protocol(HTTP/1.1)
		std::string	_path_info;//infos path supplementaires
		std::string	_request_uri;//URI de la requete en cours
};
#endif