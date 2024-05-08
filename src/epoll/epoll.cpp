/**
 * Epoll stuff goes here.
 * @todo remove setup epoll from server class and create new Epoll class with public methods to add fds to the instance etc.
*/

//epoll_events is a data type used in Linux programming.
//It's used to describe which events the application is interested in 
//and any associated user data with the file descriptors that are being monitored by epoll.

/* 
struct epoll_event {
    uint32_t events;      Epoll events
    epoll_data_t data;    User data variable 
};
*/

#include "../include/epoll.hpp"

Epoll::Epoll()
{
    _epollfd = epoll_create1(0);
    if (_epollfd == -1)
    {
        throw (WebservException("Failed to create epoll file descriptor"));
    }
}

Epoll::~Epoll()
{
    close(_epollfd);
}

void Epoll::addFd(int fd)
{
    struct epoll_event event {};
}

void Epoll::modifyFd(int fd)
{

}

void Epoll::removeFd(int fd)
{

}

std::vector<epoll_event> Epoll::wait_events(int max_events = CLI_LIMIT, int timeout = -1)
{

}

