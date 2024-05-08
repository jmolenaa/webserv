/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   epoll.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 18:13:11 by yizhang       #+#    #+#                 */
/*   Updated: 2024/05/08 18:13:12 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


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

/* 
example:
epoll.addFd(newSocket, EPOLLIN | EPOLLET);
// add new socket with edge-triggered bahavior
 */

void Epoll::addFd(int fd, uint32_t events)
{
    struct epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event))
    {
        throw (WebservException("Failed to add file descriptor in epoll"));
    }
}

void Epoll::modifyFd(int fd, uint32_t events)
{
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
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        throw (WebservException("Failed to remove file descriptor in epoll"));
    }
}

std::vector<epoll_event> Epoll::wait_events(int timeout = -1)
{
    std::vector<epoll_event> events(CLI_LIMIT);
    int num_events = epoll_wait(_epollfd, events.data(), CLI_LIMIT, timeout );
    if (num_events == -1)
    {
        throw (WebservException("Epoll wait_event error"));
    }
    events.resize(num_events);
    return events;
}

//need epoll function for newconnection 
//need epoll funciton for request
