/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   recipe.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/22 14:57:32 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:12:45 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECIPE_HPP
# define RECIPE_HPP

# include <array>
# include <string>
# include <unordered_map>

# include "defines.hpp"
# include "log.hpp"

struct Recipe
{
	std::string	page;
	std::string root;
	std::string	index;
	short		allowedMethods;
	size_t		maxBodySize;
	bool		autoindex;
	
	std::pair<status, std::string>	redir;
	std::array<std::string, COUNT>	errorPaths;

	Recipe() :page("root"), root("root"), index("index.html"),
		allowedMethods(GET | POST | DELETE), maxBodySize(MAX_BODY_SIZE),
		autoindex(false), redir(OK, ""),
		errorPaths({
		"root/index.html",
		"root/status/301.html",
		"root/status/302.html",
		"root/status/400.html",
		"root/status/403.html",
		"root/status/404.html",
		"root/status/405.html",
		"root/status/411.html",
		"root/status/415.html",
		"root/status/500.html"
	}) {Log::getInstance().print("Recipe prepared for page: " + page);}

	Recipe(const Recipe& other)
	{
		if (this == &other)
			return;
		this->page = other.page;
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
	
	Recipe& operator=(const Recipe& other)
	{
		if (this == &other)
			return (*this);
		this->page = other.page;
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

typedef	std::unordered_map<std::string, Recipe> Cookbook;

#endif