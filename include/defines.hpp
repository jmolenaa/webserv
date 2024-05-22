/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   defines.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/26 17:17:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/22 13:10:20 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define CYAN "\x1b[36m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define RESET "\x1b[0m"

# define PORT 8080
# define CLI_LIMIT 10
# define BUF_LIMIT 1024
# define MAX_BODY_SIZE 100000000

# include <array>
# include <string>

typedef enum {
	GET = 1,
	POST = 2,
	DELETE = 4,
	OTHER = 8
} 	method;

/**
 * NB!!! If adding to this enum:
 * remember to update _errorPaths in Locaiton constructor below
 * and status/status.cpp constructor
*/
typedef enum
{
	OK, //200
	BAD, //400
	// FORBIDDEN, //403
	NOTFOUND, //404
	METHODNOTALLOWED, //405
	LENGTHREQUIRED, //411
	// TOOLARGE, //413
	UNSUPPORTED, //415
	INTERNALERR, //500
	COUNT
} 	status;

struct Location
{
	std::string	path;
	std::string root;
	std::string	index;
	short		allowedMethods;
	size_t		maxBodySize;
	bool		autoindex;
	
	std::pair<bool, std::string>	redir;
	std::array<std::string, COUNT>	errorPaths;

	Location() :path("root"), root("root"), index("index.html"),
		allowedMethods(GET | POST | DELETE), maxBodySize(MAX_BODY_SIZE),
		autoindex(false), redir(false, ""), 
		errorPaths({
		"root/index.html"
		"root/400.html",
		"root/404.html",
		"root/405.html",
		"root/411.html",
		"root/415.html",
		"root/500.html"
	}) {}

	Location(const Location& other) = default;
	
	// Location operator=(const Location& other)
	// {
	// 	if (this == &other)
	// 		return (*this);
	// 	this->_path = other._path;
	// 	this->_root = other._root;
	// 	this->_index = other._index;
	// 	this->_allowedMethods = other._allowedMethods;
	// 	this->_maxBodySize = other._maxBodySize;
	// 	this->_autoindex = other._autoindex;
	// 	this->_redir.first = other._redir.first;
	// 	this->_redir.second = other._redir.second;
	// 	for (int i = 0; i < COUNT; i++)
	// 		this->_errorPaths[i] = other._errorPaths[i];
	// 	return (*this);
	// }
};

#endif
