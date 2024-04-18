/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   .cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:10:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 14:39:17 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "epoll.hpp"

Epoll::Epoll()
{
	_epollfd = epoll_create(CLI_LIMIT);
	if (_epollfd < 0)
	{
		std::cerr << "Failed to create epoll instance:" << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
};

Epoll::~Epoll()
{
	if (_epollfd > 0)
		close(_epollfd);
}

void	Epoll::add(int fd)
{
	epoll_event event{};
	event.events = EPOLLIN;
	event.data.fd = fd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		std::cerr << "Failed to add to epoll: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

int	Epoll::wait(epoll_event *events)
{
	_eventCount = epoll_wait(_epollfd, events, CLI_LIMIT, -1);
	if (_eventCount == -1)
	{
		std::cerr << "Epoll failed to wait: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	return (_eventCount);
}