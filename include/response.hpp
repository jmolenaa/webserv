/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/07 12:57:00 by dliu          ########   odam.nl         */
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
		INDEX,
		HTML,
		PHP,
		FOLDER
	}	filetype;

	public:
		explicit Response(std::string& response);
		~Response() = default;

		std::string		getResponse();
	
	private:
		Status			_status;
		// Location		*locations;
		std::string 	_path;
		filetype		_filetype;
		std::ifstream	_file;
		std::string		_header;
		std::string		_body;

		void	_populateBody();
		void	_generateHeader();

		void	_extractFileType();
		void	_openFile();

		void	_populateError();
		void	_populateHtml();
		void	_executeCGI(); //move this to CGI?
		
		std::string _getDateTime();
		std::string	_getType();

	// Hidden orthodox canonical shit
		// Response& operator=(const Response& other) = default;
		Response() = default;
};

#endif