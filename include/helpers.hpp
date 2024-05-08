/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 18:59:58 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/07 15:24:26 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "fstream"
#include "cstdio"

#ifndef HELPERS_HPP
# define HELPERS_HPP

class Helpers
{
	public:
		static std::string		_keyValueFind(std::string string, std::string key, char delim);

	private:
		Helpers() = default;
};

#endif