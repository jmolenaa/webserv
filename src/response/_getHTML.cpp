/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _getHTML.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/06 12:45:44 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/13 13:00:39 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

void	Response::_getHtml()
{
	std::string	filePath = _location.getRoot() + _path;
	std::ifstream _file(filePath);
	std::string line;
	if (_file.is_open())
	{
		while (std::getline(_file, line))
			_body += line + "\n";
		_file.close();
	}
	else
		return (_status.updateState(Status::NOTFOUND));
}