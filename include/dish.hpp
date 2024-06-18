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

class Order;

class Dish : public FdHandler
{
	typedef enum
	{
		NONE,
		HTML,
		FOLDER
	}	filetype;

	public:
		Dish(Status& status, Order const& order, Recipe recipe, void* restaurantPointer, void* customerPointer);
		Dish() = delete;
		~Dish();

		void		input(int eventFD) override;
		void		output(int eventFD) override;

		std::string	getDish();
		void		doMethod();

	private:
		Status&			_status;
		Order const&	_order;
		Recipe			_recipe;
		std::string		_header;
		std::string		_body;
		char			_buffer[BUF_LIMIT];
		int				_pipeFDs[2];
		void*			_customerPointer;

		void	_doPipe();
		void	_trashDish();

		void	_doError();
		void	_get();
		void	_post();
		void	_delete();
		void	_generateHeader();

		class CGI
		{
			public: 
				explicit CGI(Dish& parent);
				CGI() = delete;
				
				int	execute();
			
			private:
				Dish&		_dish;
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