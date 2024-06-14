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

#ifndef DISH_HPP
# define DISH_HPP

# include <fcntl.h>

# include "defines.hpp"
# include "status.hpp"
# include "recipe.hpp"
# include "fdHandler.hpp"
# include "order.hpp"

class Dish : public FdHandler
{
	typedef enum
	{
		NONE,
		HTML,
		FOLDER
	}	filetype;

	public:
		Dish(Order& order, Recipe& recipe, void* restaurantPointer);
		Dish() = delete;
		~Dish();

		void		input(int eventFD) override;
		void		output(int eventFD) override;

		std::string tmpGetResponse();

	private:
		Order&			_order;
		Recipe&			_recipe;
		std::string		_header;
		std::string		_body;
		char			_buffer[BUF_LIMIT];

		std::string		_sendMessage;
		ssize_t			_sendSize;
		size_t			_sendPos;

		void	_doMethod();
		void	_readyDish();
		void	_serveDish();
		void	_generateHeader();

		void		_get();
		filetype	_extractFileType();

		void	_post();
		void	_delete();

		class CGI
		{
			public: 
				explicit CGI(Dish& parent);
				CGI() = delete;
				
				int	execute();
			
			private:
				Dish&		_parent;
				std::string _query;
				std::string	_path;
				int			_inFD[2];
				int			_outFD[2];
				pid_t		_pid;

				char**						_env;
				std::vector<std::string>	_vec;
				void		_vectorToEnv(const std::vector<std::string>& vec);	
				void		_freeEnv();
				
				void 		_execChild();
				void		_setEnv();
				void		_execError(std::string what, std::string why);
		};
};

#endif