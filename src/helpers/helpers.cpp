/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 19:01:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 17:08:20 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"
#include "iostream"

/**
 * Finds and returns the substr in string found after the "key", terminated / deliminated by 'delim'
*/
std::string Helpers::_keyValueFind(std::string string, std::string key, char delim)
{
	size_t start = string.find(key);
	if (start == std::string::npos)
		return ("");
	start += key.length();
	size_t end = string.find_first_of(delim, start);
	return(string.substr(start, end - start));
}

int	Helpers::_getFileFd(std::string path, filetype type)
{
	int	fd;

	//open path. if it doesn't exist, open default for the type
}