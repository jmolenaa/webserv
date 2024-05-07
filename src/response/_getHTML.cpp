/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _getHTML.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/06 12:45:44 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/07 13:12:02 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

void	Response::_getHtml()
{
	_openFile();
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

void Response::_openFile()
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
	_file.open(root);
}