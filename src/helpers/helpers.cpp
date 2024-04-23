/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 19:01:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/23 19:05:35 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.hpp"

/**
 * Finds and returns the substr in string found after the "key", terminated / deliminated by 'delim'
*/
std::string Helpers::_keyValueFind(std::string string, std::string key, char delim)
{
	size_t	start;
	size_t	end;

	start = string.find(key);
	if (start == std::string::npos)
		return ("");
	start += key.length();
	start += key.find_first_not_of(delim, start);
	end = key.find_first_of(delim, start);
	return(key.substr(start, end - start));
}
