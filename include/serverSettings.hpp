/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverSettings.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:38 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/08 11:06:38 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by jmolenaa on 8-5-24.
//

#ifndef WEBSERV_SERVERSETTINGS_HPP
#define WEBSERV_SERVERSETTINGS_HPP

#include <string>
#include "location.hpp"

class ServerSettings {

public:
	ServerSettings();

	uint16_t				getPort() const;
	uint32_t				getAddress() const;
	std::string				getName() const;

	struct	loc {
		std::string	_path;
		std::string _root;
		std::string	_index;
		uint		_allowedMethods;
		uint		_maxBodySize;

		std::string	_errorPaths[20];

		std::string	_redir;
		bool		_autoindex;
	};

	std::vector<loc>	getLocs() const;

private:
	uint16_t				_port;
	uint32_t				_address;
	std::string				_name;
	std::vector<loc>		_locs;

};


#endif //WEBSERV_SERVERSETTINGS_HPP
