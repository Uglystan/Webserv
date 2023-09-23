/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourdon <abourdon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:26:10 by abourdon          #+#    #+#             */
/*   Updated: 2023/09/23 13:26:58 by abourdon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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

class Response
{
	public:
		Response(std::string request);
		~Response(void);

		std::string	get_response(void);
		std::string	find_method(void);
		void	find_path(void);
		void	create_header(void);
		void	create_body();
		void	init_Error_code(void);
		void	find_error_code(void);
		void	body_error_page(void);
		void	cleanHeader(void);
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
		int		_code;
		int		_bool;
		std::map<int, std::string>	_errors;
};