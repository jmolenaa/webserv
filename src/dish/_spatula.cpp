/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/05 16:54:02 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_doMethod(method meth, Order& order)
{
	switch (meth)
	{
		case (GET):
			_get();
			break;
		case (POST):
			_post(order);
			break;
		case (DELETE):
			_delete(order);
			break;
		default:
			_status.updateState(METHODNOTALLOWED);
			break;
	}
}

void Dish::_readFile(const char* filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		_body += "INTERNAL ERROR: No default page found for error " + _status.getStatMessage() + "!\n";
		_status.updateState(INTERNALERR);
	}
	
		//DO EPOLL STUFF HERE
		
	char 	buffer[BUF_LIMIT] = "";
	
	ssize_t	count = read(fd, buffer, BUF_LIMIT);
	while (count)
	{
		if (count < 0)
		{
			_status.updateState(INTERNALERR);
			break;
		}
		std::string append(buffer);
		_body += append;
		count = read(fd, buffer, BUF_LIMIT);
	}
	close(fd);
}

/**
 * gets body with appropriate error file
*/
void	Dish::_getError()
{
	_body = "\r\n";
	std::string errfile = _recipe.errorPaths[_status.getState()];

	_readFile(errfile.c_str());
}
