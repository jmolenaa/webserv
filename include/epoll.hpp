/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   epoll.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 18:13:18 by yizhang       #+#    #+#                 */
/*   Updated: 2024/05/08 18:13:21 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
#define EPOLL_HPP

# include <sys/epoll.h> 				//epoll
# include <unistd.h> 					//close
# include <vector>

# include "webserv.hpp"					//CLI_LIMIT
# include "webservException.hpp" 		//exception

class Epoll
{
	public:
		Epoll();										//create epoll instance
		~Epoll();										//close epollfd

		void addFd(int fd, uint32_t events); 			//addition of sockets
		void modifyFd(int fd, uint32_t events);			//modification of sockets
		void removeFd(int fd);							//removal of sockets
		//need epoll function for newconnection 
		//need epoll funciton for request
		std::vector<epoll_event> wait_events(int timeout);
	private:
		int _epollfd;
};

/* 
 concept:
 1. Create an epoll instance using epoll_create1();
 2. Populate an epoll_event structure and ass it to the epoll instance 
 with epoll_ctl(), specifying whether to add, modify, or remove file descriptors and their event masks.
 3. Use epoll_wait() to wait for events on the epoll instance. This function will return
 a list of epoll_event structure for file descriptors that have their specified events pending
*/

/* 
Todo yixin:
1. add Epoll class in _severHandlers.cpp
2. add Epoll class in sever.cpp
3. add Epoll class in _setup.cpp
 */

#endif