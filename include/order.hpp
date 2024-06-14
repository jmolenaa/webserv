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
# include "fdHandler.hpp"
# include "defines.hpp"
# include "status.hpp"

class Order : public FdHandler
{
	public:
		Order(void* waiterPointer, int fd, void* restaurantPointer);
		Order() = delete;
		Order(Order& other) = delete;
		~Order();

		void		input(int eventFD) override;
		void		output(int eventFD) override;
		
		method	   	getMethod() const;
		std::string getPath() const;
		uint    	getTable() const;
		uint	    getLength() const;
		std::string	getType() const;
		std::string getHostname() const;
		std::string getBody() const;
		std::string getOrder() const; //check maxBodySize in dafault recipe unless overwritten

		Status		status;
	private:
		void*		_wP;
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