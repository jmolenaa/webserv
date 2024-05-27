/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _get.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:17:28 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

void Response::_get()
{
	_filetype = _extractFileType();
	switch (_filetype)
	{
	case (HTML):
		_getHtml();
		break;
	case (PY):
		_executeCGI(); //might get removed and only support it in POST
		break;
	case (FOLDER):
		_listFolder();
		break;
	default:
		_status.updateState(UNSUPPORTED);
		break;
	}
}

Response::filetype	Response::_extractFileType()
{
	size_t	dir = _path.find_last_of('/');
	if (dir == std::string::npos)
		return (_status.updateState(BAD), NONE);
	size_t pos = _path.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = _path.substr(pos + 1);
		if (type == "html")
			return (HTML);
		else if (type == "py")
			return (PY);
		else
			return (NONE);
	}
	else
	{
		if (_location.autoindex)
			return (FOLDER);
		_path += _location.index;
		return (HTML);
	}
}

/**
 * @todo Needs to go through epoll, figure out root stuff for locations
*/
void	Response::_getHtml()
{
	std::string	filePath = _location.root + _path;
	
	int fd = open(filePath.c_str(), O_RDONLY);
	if (fd == -1)
		return (_status.updateState(NOTFOUND));

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
		std::string append = std::string(buffer);
		_body += append.substr(0, count);
		count = read(fd, buffer, BUF_LIMIT);
	}
	close(fd);
}

/**
 * @todo this.
*/
void Response::_listFolder()
{
	_body += "Totally listing the directory here:\nroot/\ntoask/\nsike just kidding\n";
}