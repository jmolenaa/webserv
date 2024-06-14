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
#include "webservException.hpp"

Cook::Cook() : _table(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost")
{
	Log::getInstance().print("default Cook hired with name: " + _name);
}

// Cook::Cook(uint16_t table, uint32_t address, std::string& name, Cookbook& cookbook)
// 	: _table(table), _address(address), _name(name), _cookbook(cookbook)
// {
// 	Log::getInstance().print("custom Cook hired with name: " + _name);
// }

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

void Cook::setName(std::string newName) {
	this->_name = std::move(newName);
}

void Cook::setTable(uint16_t newTable) {
	 this->_table = newTable;
}

void Cook::setAddress(uint32_t newAddress) {
	this->_address = newAddress;
}

void Cook::addToCookbook(Recipe const& newRecipe) {

	// checking if recipe is already in the cookbook
	if (this->_cookbook.count(newRecipe.page) != 0) {
		throw WebservException("Webserv: configuration file: location with path '" + newRecipe.page + "' is used twice\n");
	}

	this->_cookbook[newRecipe.page] = newRecipe;
}

Recipe Cook::getRecipe(std::string page) const
{
	const Recipe* recipe = _cookbook.find(page);
	if (recipe == nullptr)
		recipe = _cookbook.begin();
	return (*recipe);
}
