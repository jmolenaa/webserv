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
# include "status.hpp"
# include "dish.hpp"

class Order : public FdHandler
{
	public:
		Order(void* wait, int fd);
		Order() = delete;
		Order(Order& other) = delete;
		~Order();

		status		input(int eventFD) override;
		status		output(int eventFD) override;
		
		method	   	getMethod() const;
		std::string getPath() const;
		uint    	getTable() const;
		uint	    getLength() const;
		std::string	getType() const;
		std::string getBody() const;
		std::string getOrder() const; //check maxBodySize in dafault recipe unless overwritten

		void*		waiter;

	private:
		int			_orderFD;
		Status		_status;
	
		bool		_done;
		char		_buffer[BUF_LIMIT - 1];
		std::string	_bufStr;
		
		std::string _header;
		method		_method;
		std::string _path;
		std::string	_hostname;
		uint		_table;
		uint		_contentLength;
		std::string _contentType;
		std::string	_body;
		
		Dish*		_dish;
		
		void	_extractHeader();
		void	_parseHeader();
		void 	_extractMethod();
		void	_extractPath();
		void	_extractHost();
		void	_extractBody();
		void	_printData();
		
		std::string		_extractValue(std::string string, std::string key, char delim);
};

#endif