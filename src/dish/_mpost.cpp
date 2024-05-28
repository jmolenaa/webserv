/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mpost.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:45:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/28 13:57:00 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_post(Order& order)
{
	std::string body = order.getBody();
	_body += "You tried to post: '" + body + "'\nWell, nice try I guess.";
}

void Dish::_executeCGI()
{
	_body += "CGI STUFF NOT YET IMPLEMENTED";
}
