/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _methods.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 14:14:17 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"
#include "sstream"
#include "order.hpp"

/**
 * @todo autoindex stuff
 * 
 */
void Dish::_get()
{
	std::string page = _recipe.root + _order.getPath();
	size_t pos = page.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = page.substr(pos + 1);
		if (type != "html")
			return (_order.status.updateState(UNSUPPORTED));
	
		_inFD = open(page.c_str(), O_RDONLY);
	}
	else
	{
		//if !isDirectory()
			//return (_status.updateState(UNSUPPORTED));

		// if (_recipe.autoindex)
			 //do CGI stuff here to list directory contents
		// else
		// {
			std::string index = page + _recipe.index;
			_inFD = open(index.c_str(), O_RDONLY);
		// }
	}
	if (_inFD < 0)
		return (_order.status.updateState(NOTFOUND));
}

void Dish::_post()
{
	if (_order.getPath().find("/cgi-bin/post.cgi") != 0
		&& _order.getPath().find("/cgi-bin/upload.cgi") != 0)
		return (_order.status.updateState(FORBIDDEN));
	else
	{	
		CGI cgi(*this);
		_inFD = cgi.execute();
	}
}

void Dish::_delete()
{
    if (_order.getPath().find("/cgi-bin/delete.cgi") != 0)
        return (_order.status.updateState(FORBIDDEN));
    else
	{
		CGI cgi(*this);
		_inFD = cgi.execute();
	}
}
