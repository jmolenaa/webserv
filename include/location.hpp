/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/02 11:51:42 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/03 16:13:03 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include "status.hpp"

#ifndef LOCATION_HPP
# define LOCATION_HPP

//moved status code enum to status.hpp

class Location
{
	private:
		std::string	_path; //localhost:PORT/dir/folder/file ,_ the /dir/folder part. Replace with root then start searching from the end of the path until a directory is found, else error
		std::string _root;
		std::string	_index;
		uint		_allowedMethods;
		uint		_maxBodySize;
		
		std::string	_errorPaths[20]; //fixed number until we figure out pairs

		std::string	_redir; //pair for if permanent or not
		bool		_autoindex; //off by default
		//potentially cgi stuff also goes here

	public:
		Location();
		~Location() = default;

		//getters
};

#endif