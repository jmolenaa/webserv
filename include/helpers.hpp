/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 18:59:58 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 17:08:10 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class Helpers
{
	public:
		static std::string	_keyValueFind(std::string string, std::string key, char delim);
		static int			_getFileFd(std::string path, filetype type);

	private:
		Helpers() = default;
};