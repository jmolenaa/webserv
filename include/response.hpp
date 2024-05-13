/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/13 12:51:31 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "defines.hpp"
# include "request.hpp"
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
	}	filetype;

	public:
		explicit Response(Request& request, Location& location);
		~Response() = default;

		std::string		getResponseMessage();
	
	private:
		Status			_status;
		Location&		_location;
		std::string 	_path;
		std::string		_header;
		std::string		_body;
		filetype		_filetype;

		filetype	_extractFileType();

		void	_get();
		void	_getError();
		void	_getHtml();

		void	_post();
		void	_delete();
		void	_executeCGI(); //move this to CGI?
		
		void		_generateHeader();
		std::string	_getType();
		std::string _getDateTime();

	// Hidden orthodox canonical shit
		Response& operator=(const Response& other) = delete;
		Response() = delete;
};

#endif