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
		FOLDER
	}	filetype;

	public:
		explicit Dish(Epoll& epoll, Order& order, Recipe& recipe);
		Dish() = delete;
		~Dish() = default;

		std::string		getMeal();
	
	private:
		Epoll&			_epoll;
		Order&			_order;
		Status			_status;
		Recipe			_recipe;
		std::string		_dish;
		std::string		_header;
		std::string		_body;

		void	_doMethod(method m);
		void	_fileToBody(const char* filename);
		void	_generateHeader();

		void		_get();
		filetype	_extractFileType();

		void	_post();
		void	_delete();

		class CGI
		{
			public: 
				explicit CGI(Order& order);
				CGI() = delete;
				
				std::string	execute();
			
			private:
				Order&		_order;
				std::string	_cgiPath;
				std::string _filename;
				int			_fd;
				char*		_env;

				std::string _generateFilename();
				void 		_execChild();
				void		_setEnv();
		};
};

#endif