/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/24 14:08:53 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "server.hpp"
# include "helpers.hpp"

# define BUFFERSIZE 1024

/**
 * Parses and contains request data such as method, hostname, port, and body if any
 * Use: getMethod(), getHostname(), getPort(), getLength(), getBody()
*/
class Request
{
	public:
		Request(char *request);
		~Request() = default;
		

		std::string	getMethod();
		std::string	getHostname();
		int			getPort();
		int			getLength();
		std::string	getBody();

		void		printData();

	private:
		int		_fd;
		int		_start;
		int		_pos;
	
		std::string	_request;
		std::string	_method;
		std::string	_hostname;
		uint		_port;
		uint		_contentLength;
		std::string	_body;

		void 	_extractMethod();
		void	_extractHost();
		void	_extractBody();

	// Hidden orthodox canonical shit
		Request& operator=(const Request& other) = default;
		Request();
};

#endif