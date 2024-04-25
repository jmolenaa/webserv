/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 18:32:24 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "server.hpp"

/**
 * 
*/
class Response
{
	public:
		explicit Response(std::string& response);
		~Response() = default;

		std::string getResponse();
	
	private:
		std::string 	_path;
		filetype		_type;
		std::string		_message;

		void	_findFileType();
		void	_generateHeader();
		void	_populateContent();

	// Hidden orthodox canonical shit
		Response& operator=(const Response& other) = default;
		Response() = default;
};

#endif