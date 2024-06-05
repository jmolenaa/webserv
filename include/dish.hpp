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
		Recipe			_recipe;
		std::string 	_page;
		std::string		_header;
		std::string		_body;
		filetype		_filetype;

		filetype	_extractFileType();

		void	_doMethod(method meth, Order& order);
		void	_readFile(const char* filename);
		void	_getError();

		void	_get();
		void	_getHtml();
		void	_listFolder();
		
		void	_post(Order& order);
		void	_postExecCGI();
		void 	_postCGIChild(int fd);

		void	_delete(Order& order);
		void	_deleteExecCGI();
		void	_deleteCGIChild(int fd);

		void		_generateHeader();
		std::string	_getType();
		std::string _getDateTime();
};

#endif