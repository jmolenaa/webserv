/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/22 14:57:32 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:12:45 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <array>
# include <string>
# include <unordered_map>
# include "defines.hpp"
# include "log.hpp"

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
	}) {Log::getInstance().print("Location constructed with path: " + path);}

	Location(const Location& other)
	{
		if (this == &other)
			return;
		this->path = other.path;
		this->root = other.root;
		this->index = other.index;
		this->allowedMethods = other.allowedMethods;
		this->maxBodySize = other.maxBodySize;
		this->autoindex = other.autoindex;
		this->redir.first = other.redir.first;
		this->redir.second = other.redir.second;
		for (int i = 0; i < COUNT; i++)
			this->errorPaths[i] = other.errorPaths[i];
		return;
	}
	
	Location& operator=(const Location& other)
	{
		if (this == &other)
			return (*this);
		this->path = other.path;
		this->root = other.root;
		this->index = other.index;
		this->allowedMethods = other.allowedMethods;
		this->maxBodySize = other.maxBodySize;
		this->autoindex = other.autoindex;
		this->redir.first = other.redir.first;
		this->redir.second = other.redir.second;
		for (int i = 0; i < COUNT; i++)
			this->errorPaths[i] = other.errorPaths[i];
		return (*this);
	}
};

typedef	std::unordered_map<std::string, Location> Locations;

#endif