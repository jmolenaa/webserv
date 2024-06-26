/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   concierge.cpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 18:05:12 by yizhang       #+#    #+#                 */
/*   Updated: 2024/06/26 18:05:39 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* 
epoll_events is a data type used in Linux programming.
It's used to describe which events the application is interested in 
and any associated user data with the file descriptors that are being monitored by epoll.

struct epoll_event {
    uint32_t events;      Concierge events
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
#include <cstring>

#include "concierge.hpp"
#include <iostream>
#include "log.hpp"
#include <fcntl.h>

Concierge::Concierge()
{
    _epollfd = epoll_create(CLI_LIMIT);
    _numEvents = 0;
    if (_epollfd == -1)
    {
        throw (WebservException("Failed to create epoll" + std::string(std::strerror(errno))));
    }
}

Concierge::~Concierge()
{
    if (_epollfd > 0)
        close(_epollfd);
}

void Concierge::addFd(int fd, uint32_t events)
{
    Log::getInstance().print("Concierge is monitoring fd " + std::to_string(fd));
    epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) == -1) {
        throw WebservException("Could not add fd " + std::to_string(fd) + " to epoll: " + std::string(std::strerror(errno)));
    }
}

void Concierge::modifyFd(int fd, uint32_t events)
{
    std::cout<<"epoll modity fd"<<std::endl;
    struct epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &event) == -1)
    {
        throw (WebservException("Failed to modify fd " + std::to_string(fd) + "  in epoll"));
    }
}

void Concierge::removeFd(int fd)
{
    Log::getInstance().print("Concierge will stop monitoring fd " + std::to_string(fd));
    if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
       Log::getInstance().printErr("Could not remove fd " + std::to_string(fd) + " from epoll: " + std::string(std::strerror(errno)));
    }
}

void Concierge::wait(int timeout, epoll_event *events)
{
    _numEvents = epoll_wait(_epollfd, events, CLI_LIMIT, timeout);
    Log::getInstance().print("Concierge is waiting on " + std::to_string(_numEvents) + " events");
    if (_numEvents < 0)
    {
        throw (WebservException("Epoll wait error" + std::string(std::strerror(errno))));
    }
}

int Concierge::getNumEvents()
{
    if (_numEvents == -1)
    {
        throw (WebservException("Can not get numEvents"));
    }
    return _numEvents;
}

int Concierge::set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "fcntl F_GETFL error" << std::endl;
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        std::cerr << "fcntl F_SETFL error" << std::endl;
        return -1;
    }
    return 0;
}

// int Concierge::getEpollFd()
// {
//     if (_epollfd == -1)
//     {
//         throw (WebservException("Can not get epollFd"));
//     }
//     return _epollfd;
// }

