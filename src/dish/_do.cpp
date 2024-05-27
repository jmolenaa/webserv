/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _do.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 15:30:24 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_doMethod(method meth, Order& order)
{
	switch (meth)
	{
		case (GET):
			_get();
			break;
		case (POST):
			_post(order);
			break;
		case (DELETE):
			_delete(order);
			break;
		default:
			_status.updateState(METHODNOTALLOWED);
			break;
	}
}

void Dish::_post(Order& order)
{
	std::string body = order.getBody();
	_body += "You tried to post: '" + body + "'\nWell, nice try I guess.";
}

void Dish::_delete(Order& order)
{
	std::string path = order.getPath();
	_body += "You tried to delete: '" + path + "'\nNice try but deleting is not yet implemented.";
}

void Dish::_executeCGI()
{
	_body += "CGI STUFF NOT YET IMPLEMENTED";
}