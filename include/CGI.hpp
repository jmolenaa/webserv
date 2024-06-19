/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/19 13:35:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/19 13:54:58 by dliu          ########   odam.nl         */
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
		CGI() = delete;
		~CGI();
		
		void	execute();
		
		void	input(int eventFD) override;
		void	output(int eventFD) override;
	
	private:
		Dish&		_dish;
		std::string _query;
		std::string	_path;
		int			_CGIInputPipe[2];
		int			_CGIOutputPipe[2];
		pid_t		_pid;
		
		ssize_t		_pos;
		std::string _message;
		char		_buffer[BUF_LIMIT];

		char**						_env;
		std::vector<std::string>	_vec;

		void		_vectorToEnv(const std::vector<std::string>& vec);	
		void		_freeEnv();
		void		_setEnv();
		void 		_execChild();
		void		_closePipes(std::string what, std::string why);
};

#endif