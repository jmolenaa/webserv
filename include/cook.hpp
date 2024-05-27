/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cook.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:38 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/27 12:45:33 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOK_HPP
#define COOK_HPP

# include <string>
# include <unordered_map>
# include "recipe.hpp"

class Cook {
	public:
		explicit	Cook();
		explicit	Cook(uint16_t port, uint32_t address, std::string& name, Cookbook& cookbook);
		explicit	Cook(const Cook& other);
		Cook& 		operator=(const Cook& other);

		std::string		getName() const;
		uint16_t		getPort() const;
		uint32_t		getAddress() const;
		Cookbook		getCookbook() const;

		Recipe			getRecipe(std::string path) const;
	
	private:
		uint16_t	_port;
		uint32_t	_address;
		std::string	_name;
		Cookbook	_cookbook;
};

typedef std::unordered_map<std::string, Cook> Kitchen;

#endif
