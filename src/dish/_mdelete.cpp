/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _delete.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:48:44 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/28 13:49:09 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_delete(Order& order)
{
	std::string page = order.getPath();
	_body += "You tried to delete: '" + page + "'\nNice try but deleting is not yet implemented.";
}