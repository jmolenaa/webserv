/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:06:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/16 14:22:53 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <vector>
#include "server.hpp"

class	Webserv {

public:
	Webserv() = default;
	void	run();

private:

	std::vector<Server>	_servers;

};

#endif