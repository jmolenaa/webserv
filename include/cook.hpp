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
# include "orderedMap.hpp"
# include "recipe.hpp"

class Cook {
	public:
		Cook();
		Cook(const Cook& other);
		Cook& 		operator=(const Cook& other);
		
		void			setName(std::string);
		void			setTable(uint16_t);
		void			setAddress(uint32_t);
		void			addToCookbook(Recipe const& newRecipe);

		std::string		getName() const;
		uint16_t		getTable() const;
		uint32_t		getAddress() const;
		Cookbook		getCookbook() const;
		Recipe			getRecipe(std::string page) const;
	
	private:
		uint16_t	_table;
		uint32_t	_address;
		std::string	_name;
		Cookbook	_cookbook;
};

typedef orderedMap<std::string, Cook> Kitchen;

#endif
