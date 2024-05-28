/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   order.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 14:07:41 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ORDER_HPP
# define ORDER_HPP

# include "defines.hpp"
# include <string>

/**
 * Parses and contains order data such as method, hostname, table, and body if any
 * Use: getMethod(), getHostname(), getTable(), getLength(), getBody()
*/
class Order
{
	public:
		explicit Order(char *order);
		Order() = delete;
		Order(Order& other) = delete;
		~Order() = default;

		method&	   		getMethod();
		std::string&    getPath();
		std::string&    getCookName();
		uint&	    	getTable();
		uint&		    getLength();
		std::string&    getBody(); //check maxBodySize in dafault recipe unless overwritten

	private:
		std::string	_order;
		std::string _header;
		method		_method;
		std::string _page;
		std::string	_hostname;
		uint		_table;
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