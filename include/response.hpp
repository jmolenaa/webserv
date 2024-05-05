/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/03 16:11:22 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# include "status.hpp"
# include "location.hpp"

class Response
{
	typedef enum
	{
		NONE,
		HTML,
		PHP,
		FOLDER
	}	type;

	public:
		explicit Response(std::string& response);
		~Response() = default;

		std::string		getResponse();
	
	private:
		Status			_status;
		// Location		*locations;
		std::string 	_path;
		std::string		_filetype;
		std::string		_header;
		std::string		_body;

		void	_populateBody();
		void	_generateHeader();

		void	_findFileType();
		void	_populateError();
		
		std::string _getDateTime();
		std::string	_getType();

	// Hidden orthodox canonical shit
		Response& operator=(const Response& other) = default;
		Response() = default;
};

#endif