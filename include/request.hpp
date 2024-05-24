/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 14:07:41 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "defines.hpp"
# include <string>

/**
 * Parses and contains request data such as method, hostname, port, and body if any
 * Use: getMethod(), getHostname(), getPort(), getLength(), getBody()
*/
class Request
{
	public:
		explicit Request(char *request);
		Request() = delete;
		~Request() = default;

		method&	   		getMethod();
		std::string&    getPath();
		std::string&    getHostname();
		uint&	    	getPort();
		uint&		    getLength();
		std::string&    getBody(); //check maxBodySize in dafault location unless overwritten

	private:
		std::string	_request;
		std::string _header;
		method		_method;
		std::string _path;
		std::string	_hostname;
		uint		_port;
		uint		_contentLength;
		std::string	_body;
		
		void	_extractHeader();
		void 	_extractMethod();
		void	_extractPath();
		void	_extractHost();
		void	_extractBody();
		void	_printData();
		
		std::string		_keyValueFind(std::string string, std::string key, char delim);
};

#endif