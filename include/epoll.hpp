/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   epoll.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 18:13:18 by yizhang       #+#    #+#                 */
/*   Updated: 2024/05/16 14:44:03 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLL_HPP
#define EPOLL_HPP

# include "defines.hpp"					//CLI_LIMIT
# include "webservException.hpp" 		//exception

# include <sys/epoll.h> 				//epoll
# include <unistd.h> 					//close
# include <vector>

//# include "server.hpp"

class Epoll
{
	public:
		Epoll();										//create epoll instance
		~Epoll();										//close epollfd

		void addFd(int fd, uint32_t events); 			//addition of sockets
		void modifyFd(int fd, uint32_t events);			//modification of sockets
		void removeFd(int fd);							//removal of sockets
		// void newconnection();
		// void request(epoll_event event,int fd);
		void wait_events(int timeout, epoll_event *events);
		int getNumEvents(); 
		int getEpollFd();
		epoll_event *getEvents();
		
	private:
		int 		_epollfd;
		int 		_numEvents;
		epoll_event *_events;
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