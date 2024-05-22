/**
 * Epoll stuff goes here.
 * @todo remove setup epoll from server class and create new Epoll class with public methods to add fds to the instance etc.
*/

/* 
epoll_events is a data type used in Linux programming.
It's used to describe which events the application is interested in 
and any associated user data with the file descriptors that are being monitored by epoll.

struct epoll_event {
    uint32_t events;      Epoll events
    epoll_data_t data;    User data variable 
};
data:
a union that can be used to store user-defined data associated with the fd.

events type include:
EPOLLIN: The associated file descriptor is available for read operations.
EPOLLOUT: The associated file descriptor is available for write operations.
EPOLLRDHUP: Stream socket peer closed the connection, or shut down writing half of the connection.
EPOLLHUP: There is a hang up happened on the associated file descriptor.
EPOLLERR: Error condition happened on the associated file descriptor.
EPOLLET: Sets the Edge Triggered behavior for the associated file descriptor. The default behavior for epoll is Level Triggered.
EPOLLONESHOT: Ensures that one and only one thread is woken when an event occurs.
*/

#include "../../include/epoll.hpp"

Epoll::Epoll()
{
    _epollfd = epoll_create(CLI_LIMIT);
    _numEvents = 0;
    if (_epollfd == -1)
    {
        throw (WebservException("Failed to create epoll file descriptor"));
    }
}

Epoll::~Epoll()
{
    if (_epollfd > 0)
        close(_epollfd);
}

/* 
example:
epoll.addFd(newSocket, EPOLLIN | EPOLLET);
// add new socket with edge-triggered bahavior
 */

void Epoll::addFd(int fd, uint32_t events)
{
    std::cout<<"epoll add fd"<<std::endl;
    epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        throw (WebservException("Failed to add file descriptor in epoll"));
    }
}

void Epoll::modifyFd(int fd, uint32_t events)
{
    std::cout<<"epoll modity fd"<<std::endl;
    struct epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        throw (WebservException("Failed to modify file descriptor in epoll"));
    }
}

void Epoll::removeFd(int fd)
{
    std::cout<<"epoll remove fd"<<std::endl;
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        throw (WebservException("Failed to remove file descriptor in epoll"));
    }
}

void Epoll::wait_events(int timeout, epoll_event *events)
{
    std::cout<<"wait for new conncetion"<<std::endl;
    _numEvents = epoll_wait(_epollfd, events, CLI_LIMIT, timeout);
    std::cout<< _numEvents<<std::endl;
    if (_numEvents < 0)
    {
        throw (WebservException("Epoll wait_event error"));
    }
}

int Epoll::getNumEvents()
{
    if (_numEvents == -1)
    {
        throw (WebservException("Can not get numEvents"));
    }
    return _numEvents;
}

int Epoll::getEpollFd()
{
    if (_epollfd == -1)
    {
        throw (WebservException("Can not get epollFd"));
    }
    return _epollfd;
}

//need epoll function for newconnection 
//need epoll funciton for request
