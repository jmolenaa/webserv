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
	_responseFd = Helpers::_getFileFd(_path, _type);
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
	_message += "html stuff";
}

std::string Response::getResponse()
{
	return (_message);
}