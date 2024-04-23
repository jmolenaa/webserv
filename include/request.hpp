/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/23 19:01:49 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "server.hpp"
# include "helpers.hpp"

# define BUFFERSIZE 1024

class Request
{
	public:
		Request(char *request);
		~Request();
		

		std::string	getReason();
		std::string	getHostname();
		int			getPort();
		int			getLength();
		std::string	getBody();

	private:
		int		_fd;
		int		_start;
		int		_pos;
	
		std::string	_request;
		std::string	_reason;
		std::string	_hostname;
		uint		_port;
		uint		_contentLength;
		std::string	_body;

		std::string _extractBody(std::string _request);

	// Hidden orthodox canonical shit
		Request& operator=(const Request& other) = default;
		Request();
};

#endif