//epoll stuff goes here

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
		Epoll();						//create epoll instance
		~Epoll();						//close epollfd

		void addFd(int fd); 			//addition of sockets
		void modifyFd(int fd);			//modification of sockets
		void removeFd(int fd);			//removal of sockets
		std::vector<epoll_event> wait_events(int max_events = CLI_LIMIT, int timeout = -1);
	private:
		int _epollfd;
};

/* 
 concept:
 1. create an epoll instance using epoll_create1();
 2. 
*/
#endif