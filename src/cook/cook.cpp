/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cook.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:40 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/27 13:13:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>

#include "cook.hpp"
#include "defines.hpp"
#include "log.hpp"

Cook::Cook()
	: _table(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost")
{
	Log::getInstance().print("default Cook hired with name: " + _name);
}

Cook::Cook(uint16_t table, uint32_t address, std::string& name, Cookbook& cookbook)
	: _table(table), _address(address), _name(name), _cookbook(cookbook)
{
	Log::getInstance().print("custom Cook hired with name: " + _name);
}

Cook::Cook(const Cook& other)
{
	if (this == &other)
		return;
	this->_table = other.getTable();
	this->_address = other.getAddress();
	this->_name = other.getName();
	this->_cookbook = other.getCookbook();
}

Cook& Cook::operator=(const Cook& other)
{
	if (this == &other)
		return (*this);
	this->_table = other.getTable();
	this->_address = other.getAddress();
	this->_name = other.getName();
	this->_cookbook = other.getCookbook();
	return (*this);
}

uint16_t Cook::getTable() const
{
	return this->_table;
}

uint32_t Cook::getAddress() const
{
	return this->_address;
}

std::string Cook::getName() const
{
	return this->_name;
}

Cookbook Cook::getCookbook() const
{
	return this->_cookbook;
}

Recipe Cook::getRecipe(std::string page) const
{
	auto recipe = _cookbook.find(page);
	if (recipe != _cookbook.end())
		return (recipe->second);
	return _cookbook.begin()->second;
}
