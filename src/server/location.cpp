/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/07 12:13:18 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/07 12:13:18 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include "webservException.hpp"
#include "location.hpp"
#include "defines.hpp"
#include <iostream>

//TODO
// questions, default path is empty?, figure out error pages initialisation and redirections remove weir dvoid stuff

static std::string	getExePath() {

	char buf[PATH_MAX];

	if (readlink("/proc/self/exe", buf, PATH_MAX) == -1){
		perror("Webserv");
		throw WebservException("Error occurred during initialisation\n");
	}

	std::string	path(buf);
	return path.substr(0, path.rfind('/'));

}

Location::Location() : _path(""), _root(getExePath()), _index("index.html"), _allowedMethods(GET | POST | DELETE),
						_maxBodySize(MAX_BODY_SIZE), _autoindex(false)
{
	(void)_allowedMethods;
	(void)_maxBodySize;
	(void)_autoindex;
}
