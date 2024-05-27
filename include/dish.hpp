/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dish.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 11:51:55 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <fcntl.h>

# include "epoll.hpp"
# include "order.hpp"
# include "status.hpp"
# include "recipe.hpp"
# include "defines.hpp"

class Dish
{
	typedef enum
	{
		NONE,
		HTML,
		PY,
		FOLDER
	}	filetype;

	public:
		explicit Dish(Epoll& epoll, Order& order, Recipe& recipe);
		Dish() = delete;
		~Dish() = default;

		std::string		getMeal();
	
	private:
		Epoll&			_epoll;
		Status			_status;
		Recipe			_location;
		std::string 	_path;
		std::string		_header;
		std::string		_body;
		filetype		_filetype;

		filetype	_extractFileType();

		void	_doMethod(method meth, Order& order);

		void	_get();
		void	_getError();
		void	_getHtml();
		
		void	_listFolder();

		void	_post(Order& order);
		void	_delete(Order& order);
		void	_executeCGI(); //move this to CGI?
		
		void		_generateHeader();
		std::string	_getType();
		std::string _getDateTime();
};

#endif