/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errors.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:47:30 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 16:36:26 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

/**
 * gets body with appropriate error file
 * @todo use epoll
*/
void	Dish::_getError()
{
	_body = "\r\n";
	std::string errfile = _location.errorPaths[_status.getState()];

	int fd = open(errfile.c_str(), O_RDONLY);
	if (fd == -1)
	{
		_body += "INTERNAL ERROR: No default path found for error " + _status.getStatMessage() + "!\n";
		_status.updateState(INTERNALERR);
	}
	
		//DO EPOLL STUFF HERE

	char 	buffer[BUF_LIMIT];
	ssize_t	count = read(fd, buffer, BUF_LIMIT);
	while (count)
	{
		if (count < 0)
		{
			_status.updateState(INTERNALERR);
			break;
		}
		_body += buffer;
		count = read(fd, buffer, BUF_LIMIT);
	}
	close(fd);
}
