/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _methods.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/06 21:15:20 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

/**
 * @todo double check routing when config parsing is done
 */
void Dish::_get()
{
	std::string page = _order.getPath();
	_dish = _recipe.root + page;
	size_t pos = page.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = page.substr(pos + 1);
		if (type != "html")
			return (_status.updateState(UNSUPPORTED));
	}
	else
	{
		if (_recipe.autoindex)
			_dish = _recipe.dir;
		else
			_dish += _recipe.index;
	}
}

void Dish::_delete()
{
    if (_order.getPath() != "/cgi/delete.cgi")
        return (_status.updateState(FORBIDDEN));
    else
	{
		CGI cgi(_order);
		_dish = cgi.execute();
	}
}

void Dish::_post()
{
    if (_order.getPath() != "/cgi/post.cgi")
        return (_status.updateState(FORBIDDEN));
    else
	{
		CGI cgi(_order);
		_dish = cgi.execute();
	}
}
