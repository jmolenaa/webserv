/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/24 14:35:52 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "helpers.hpp"

Response::Response(std::string& requestPath) : _path(requestPath)
{
	//parse path, get file, compile response
	_findFileType();
	_generateHeader();
	_populateContent();
}

void	Response::_findFileType()
{
	//get filetype from _path
	_type = HTML;
}

void	Response::_generateHeader()
{
	_message += "Header content\r\n";
}

void Response::_populateContent()
{
	_message += "\r\n";
	std::ifstream file(_path);
	std::string line;
	if (file.is_open())
	{
		while (std::getline(file, line))
			_message += line + '\n';
		file.close();
	}
	else
		_message += "Error, no file found\n";
}

std::string Response::getResponse()
{
	std::cout << "sending: '" << _message << "'" << std::endl;
	return (_message);
}