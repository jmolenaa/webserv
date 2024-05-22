/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 15:02:18 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "epoll.hpp"
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
		explicit Response(Epoll& epoll, Request& request, Location location);
		Response() = delete;
		~Response() = default;

		std::string		getResponseMessage();
	
	private:
		Epoll&			_epoll;
		Status			_status;
		Location		_location;
		std::string 	_path;
		std::string		_header;
		std::string		_body;
		filetype		_filetype;

		filetype	_extractFileType();

		void	_get();
		void	_getError();
		void	_getHtml();

		void	_post(Request& request);
		void	_delete(Request& request);
		void	_executeCGI(); //move this to CGI?
		
		void		_generateHeader();
		std::string	_getType();
		std::string _getDateTime();
};

#endif