/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   epoll.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 17:55:11 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
# define EPOLL_HPP

# include <sys/epoll.h>
# include "webserv.hpp"

class Epoll
{
	public:
		Epoll();
		~Epoll();

		void	add(int fd);
		int		wait(epoll_event *events);

	private:
		int		_eventCount;
		int		_epollfd;

	// Hidden orthodox canonical shit
		Epoll& operator=(const Epoll& other) = default;
	
};

#endif