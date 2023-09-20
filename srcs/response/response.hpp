/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abourdon <abourdon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 10:26:10 by abourdon          #+#    #+#             */
/*   Updated: 2023/09/20 16:44:51 by abourdon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>

class Response
{
	public:
		Response(std::string request);
		Response(const Response &copy);
		~Response(void);

		Response	&operator=(const Response &substitue);

		std::string	create_response(void);
		void	find_method(void);
		void	find_path(void);
		void	create_header(void);
		void	create_body();
		std::string	find_status_line(void);
		std::string	find_server(void);
		std::string	find_date(void);
		std::string	find_content_type(void);
		std::string	find_content_lenght(void);
		std::string	find_connection(void);
		std::string	find_langage(void);
		
		std::string	get_response(void) const;
		std::string	get_request(void) const;
		std::string	get_status_line(void) const;
		std::string	get_server(void) const;
		std::string	get_date(void) const;
		std::string	get_content_type(void) const;
		std::string	get_content_lenght(void) const;
		std::string	get_connection(void) const;
		void	set_response(std::string response);
		void	set_request(std::string request);
		void	set_status_line(std::string status_line);
		void	set_server(std::string server);
		void	set_date(std::string date);
		void	set_content_type(std::string content_type);
		void	set_content_lenght(std::string content_lenght);
		void	set_connection(std::string connection);

	private:
		std::string	_response;
		std::string	_request;
		std::string	_status_line;
		std::string	_server;
		std::string	_date;
		std::string	_content_type;
		std::string	_content_lenght;
		std::string	_connection;
		std::string	_path;
		std::string	_body;
		std::string	_header;
};

#endif