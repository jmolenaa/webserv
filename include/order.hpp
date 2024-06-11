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

# include <string>

# include "defines.hpp"

/**
 * Parses and contains order data such as method, hostname, table, and body if any
 * Use: getMethod(), getHostname(), getTable(), getLength(), getBody()
*/
class Order
{
	public:
		explicit Order(std::string order);
		Order() = delete;
		Order(Order& other) = delete;
		~Order() = default;

		method	   	getMethod() const;
		std::string getPath() const;
		std::string getCookName() const;
		uint    	getTable() const;
		uint	    getLength() const;
		std::string	getType() const;
		std::string getBody() const;
		std::string getOrder() const; //check maxBodySize in dafault recipe unless overwritten

	private:
		std::string	_order;
		std::string _header;
		method		_method;
		std::string _page;
		std::string	_hostname;
		uint		_table;
		uint		_contentLength;
		std::string _contentType;
		std::string	_body;
		
		void	_extractHeader();
		void 	_extractMethod();
		void	_extractPath();
		void	_extractHost();
		void	_extractContent();
		void	_printData();
		
		std::string		_keyValueFind(std::string string, std::string key, char delim);
};

#endif