/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mget.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/06 12:34:15 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_get()
{
	_filetype = _extractFileType();
	switch (_filetype)
	{
	case (HTML):
		_getHtml();
		break;
	case (FOLDER):
		_listFolder();
		break;
	default:
		_status.updateState(UNSUPPORTED);
		break;
	}
}

Dish::filetype	Dish::_extractFileType()
{
	size_t	dir = _page.find_last_of('/');
	if (dir == std::string::npos)
		return (_status.updateState(BAD), NONE);
	size_t pos = _page.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = _page.substr(pos + 1);
		if (type == "html")
			return (HTML);
		else
			return (NONE);
	}
	else
	{
		if (_recipe.autoindex)
			return (FOLDER);
		_page += _recipe.index;
		return (HTML);
	}
}

/**
 * @todo double check routing when config parsing is done
*/
void	Dish::_getHtml()
{
	std::string	filePath = _recipe.root + _page;
	
	_readFile(filePath.c_str());
}

void Dish::_listFolder()
{
	std::string	filePath = "root/dir.html";
	
	_readFile(filePath.c_str());
}