/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _getHTML.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/06 12:45:44 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/08 10:38:35 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

void	Response::_getHtml()
{
	//replace with location stuff
	std::string root = "pages/";
	switch (_filetype)
	{
		case HTML:
		{
			root += _path;
			break;
		}
		case INDEX:
		{
			root += "index.html"; //replace with locations stuff
			_filetype = HTML;
			break;
		}
		default:
			break;
	}
	std::ifstream _file(root);
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
