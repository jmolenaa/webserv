/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/07 15:12:50 by dliu          ########   odam.nl         */
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
		INDEX,
		HTML,
		PHP,
		FOLDER
	}	filetype;

	public:
		explicit Response(Request& request);
		~Response() = default;

		std::string		getResponseMessage();
	
	private:
		Status			_status;
		// Location		*locations;
		Status			_status;
		// Location		*locations;
		std::string 	_path;
		filetype		_filetype;
		std::ifstream	_file;
		std::string		_header;
		std::string		_body;
		filetype		_filetype;
		std::ifstream	_file;
		std::string		_header;
		std::string		_body;

		void	_get();
		void	_getError();
		void	_getHtml();
	
		void	_post();
	
		void	_delete();
	
		void	_executeCGI(); //move this to CGI?
		
		void	_generateHeader();
	
		void	_extractFileType();
		void	_openFile();

		
		std::string _getDateTime();
		std::string	_getType();

	// Hidden orthodox canonical shit
		// Response& operator=(const Response& other) = default;
		Response() = default;
};

#endif