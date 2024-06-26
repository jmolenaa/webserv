#ifndef CONCIERGE_HPP
#define CONCIERGE_HPP

# include "defines.hpp"					//CLI_LIMIT
# include "webservException.hpp" 		//exception

# include <sys/epoll.h> 				//epoll
# include <unistd.h> 					//close

class Concierge
{
	public:
		Concierge();										//create epoll instance
		~Concierge();										//close epollfd

		void	addFd(int fd, uint32_t events); 			//addition of sockets
		void	modifyFd(int fd, uint32_t events);			//modification of sockets
		void	removeFd(int fd);							//removal of sockets
		void	wait(int timeout, epoll_event *events);
		int 	getNumEvents(); 
		int		getEpollFd();
		int set_non_blocking(int fd);
		//epoll_event *getEvents();
		
	private:
		int 		_epollfd;
		int 		_numEvents;
};

/* 
 concept:
 1. Create an epoll instance using epoll_create1();
 2. Populate an epoll_event structure and ass it to the epoll instance 
 with epoll_ctl(), specifying whether to add, modify, or remove file descriptors and their event masks.
 3. Use epoll_wait() to wait for events on the epoll instance. This function will return
 a list of epoll_event structure for file descriptors that have their specified events pending
*/

#endif