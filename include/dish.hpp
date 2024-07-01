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
# include "CGI.hpp"

class Customer;

class Dish : public FdHandler
{
	public:
		Dish(Status& status, Order const& order, Recipe recipe, Customer& cust);
		Dish() = delete;
		~Dish() override;

		void		input(int eventFD) override;
		void		output(int eventFD) override;

		std::string	getDish();
		void		doMethod();
		void		doError();
		bool		isRightCGIExtenion(std::string const& executable) const;

		Status&			status;
		Order const&	order;
		Recipe			recipe;
		Customer&		customer;

		std::string		header;
		std::string		body;
		bool			done;
		std::string		finalPage;

	private:
		char			_buffer[BUF_LIMIT];
		int				_pipeFDs[2];
		int 			_savePipeFDs[2];
		CGI*			_CGI;
		int				_fdOfFileToRead;
		bool			_doneReading;
		ssize_t			_fileSize;

		void	_horribleError();
		bool	_thereIsAnInitialError();
		void	_handleFileError(int errorCode);
		void	_handleOutputError();

		void		_trashDish();
		void		_removeHandler(int& handlerFd);
		void		_doPipe();
		void		_writeToPipe(ssize_t count);
		void		_redirect();
		void		_get();
		bool		_isCGIRequest() const;
		void		_autoindex();
		std::string	_generateDirectoryListing();
		void		_post();
		void		_delete();
		void		_generateHeader();
};

#endif