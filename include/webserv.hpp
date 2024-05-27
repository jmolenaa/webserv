/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:06:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:04:10 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <vector>
#include "server.hpp"


class	Webserv {

public:
	explicit Webserv(std::string configFileName);
	explicit Webserv() = delete;
	Webserv(Webserv& other) = delete;
	~Webserv() = default;
	
	void	run();

private:
	Epoll 					_epoll;
	std::vector<Server*>	_servers;
};

#endif