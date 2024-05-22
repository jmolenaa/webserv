/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/22 14:57:32 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 14:59:43 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <array>
# include <string>
# include "defines.hpp"

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
		"root/index.html",
		"root/400.html",
		"root/404.html",
		"root/405.html",
		"root/411.html",
		"root/415.html",
		"root/500.html"
	}) {}

	// Location(const Location& other) = default;
	
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