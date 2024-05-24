/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 18:59:58 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 11:56:09 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <string>

class Helpers
{
	public:
		static std::string		_keyValueFind(std::string string, std::string key, char delim);

	private:
		Helpers() = default;
};

#endif