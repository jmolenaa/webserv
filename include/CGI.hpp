/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/19 13:35:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/24 12:47:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

#include <vector>
#include "defines.hpp"
#include "fdHandler.hpp"

class Dish;

class CGI : public FdHandler
{
	public:
		explicit CGI(Dish& parent);
		CGI(Dish& parent, std::string uploadCgi);
		CGI() = delete;
		~CGI() override;
		
		void	execute();
		
		void	input(int eventFD) override;
		void	output(int eventFD) override;
		void	handleHangup() override;

	private:
		Dish&		_dish;
		std::string _query;
		std::string	_path;
		std::string	_pathInfo;
		int			_CGIInputPipe[2];
		int			_CGIOutputPipe[2];
		pid_t		_pid;
		
		size_t				_pos;
		std::string const& 	_message;
		char				_buffer[BUF_LIMIT];

		char**						_env;
		std::vector<std::string>	_vec;

		bool		_thereIsAnInitialError();
		void		_setEnv();
		void 		_execChild();
		void		_closePipes();
		void		_CGIError(std::string what, std::string why);
		void		_removeHandler(int& handlerFd);
		void		_tryChangeDir();
		void		_setupEnv();
};

#endif