/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errors.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:47:30 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 14:55:42 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

/**
 * gets body with appropriate error file (currently just default 404)
 * @todo use appropriate error files based on status code and locations. 
 * Use epoll
*/
void	Response::_getError()
{
	std::string errfile = _location.errorPaths[_status.getState()];
	std::cout << "ERRFILE: " << errfile << std::endl;
	
	std::ifstream err(errfile);
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
