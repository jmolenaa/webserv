/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errors.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:47:30 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/14 15:13:06 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

/**
 * gets body with appropriate error file (currently just default 404)
 * @todo use appropriate error files based on status code and locations. 
 * If error not read and set up at start of program, go through epoll.
*/
void	Response::_getError()
{
	std::ifstream err("root/error.html");
	if (err.is_open())
	{
		std::string line;
		while (std::getline(err, line))
			_body += line + "\n";
		err.close();
	}
	else
		_body += "No defaults found for this error\n";
}
