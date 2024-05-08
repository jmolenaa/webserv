//epoll stuff goes here

#ifndef EPOLL_HPP
#define EPOLL_HPP

# include <sys/epoll.h>

class Epoll
{
	public:
		Epoll(Socket &socket);//create epoll instance
		~Epoll();

		//methods need to create
		int initEpoll(); //create epoll, if error return ERROR
		int initEpollEvents();//if error return ERROR
		int initConnection();//if error return ERROR
		int addSocket();//if error return ERROR
		int waitEpoll();//if error return ERROR
	private:
		Epoll();
};

#endif