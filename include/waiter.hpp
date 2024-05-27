/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 17:04:17 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAITER_HPP
# define WAITER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <set>
# include <iostream>
# include <unistd.h>
# include <cstring>

# include "defines.hpp"
# include "epoll.hpp"
# include "cook.hpp"

class Waiter
{
	public:
		explicit Waiter(Epoll& epoll, Kitchen kitchen);
		Waiter() = delete;
		~Waiter();

		void	work();

	private:
		Epoll&			_epoll;
		int				_waiterFd;
		Kitchen			_kitchen;

		void	_createSocket();
		void	_bindToAddress();

		void	_handleEvents(epoll_event* events, int numEvents);
		void	_welcomeCustomer();
		void	_takeOrder(int fd);
		void	_serveCustomer(int customerFd, const std::string& message);

};

#endif