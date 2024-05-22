/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errors.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:47:30 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 17:54:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

/**
 * gets body with appropriate error file
 * @todo use epoll
*/
void	Response::_getError()
{
	std::string errfile = _location.errorPaths[_status.getState()];
	std::ifstream err(errfile);
	if (err.is_open())
	{
		std::string line;
		while (std::getline(err, line))
			_body += line + "\n";
		err.close();
	}
	else
	{
		_body += "INTERNAL ERROR: No default page found for error " + _status.getStatMessage() + "!\n";
		_status.updateState(INTERNALERR);
	}
}
