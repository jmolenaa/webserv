/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _get.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/08 10:37:53 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

//gets response body
void Response::_get()
{
	_extractFileType();
	_body += "\r\n";
	if (_filetype == INDEX || _filetype == HTML)
		_getHtml();
	else if (_filetype == PHP)
		_executeCGI();
	if (_status.getState() != Status::OK)
		return (_getError());
}

//get filetype from _path
void	Response::_extractFileType()
{
	size_t	dir = _path.find_last_of('/');
	if (dir == std::string::npos)
		return (_status.updateState(Status::BAD));
	size_t pos = _path.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = _path.substr(pos + 1);
		if (type == "html")
			_filetype = HTML;
		else if (type == "php")
			_filetype = PHP;
		else
			return (_status.updateState(Status::UNSUPPORTED));
	}
	else
	{
		_filetype = INDEX;
		if (_path != "/")
		{
			_path += "/index.html";
			_filetype = HTML;
		}
	}
	// if show location is on, change filetype to folder
}
