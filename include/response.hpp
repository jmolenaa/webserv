/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 12:00:17 by dliu          ########   odam.nl         */
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
	
	private:
		std::string _path;

	// Hidden orthodox canonical shit
		Response& operator=(const Response& other) = default;
		Response() = default;
};

#endif