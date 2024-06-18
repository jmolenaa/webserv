/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   restaurant.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:09:43 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 15:25:25 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "restaurant.hpp"
#include "log.hpp"
#include "menu.hpp"

Restaurant::Restaurant(std::string const& filename)
{
	Log::getInstance().print("Constructing Restaurant instance");
	Log::getInstance().print("Parsing '" + filename);

	Menu	menu(filename);
	menu.parse();

	for (Kitchen const& kitchen : menu.getKitchens()) {
		this->_waiters.push_back(new Waiter(kitchen, this));
	}
}

Restaurant::~Restaurant()
{
	for (auto waiter : _waiters)
		delete waiter;
}

void Restaurant::run()
{
    epoll_event events[CLI_LIMIT];
	int			eventFD;

    while (true)
	{
        _epoll.wait(-1, events);
		for (int i = 0; i < _epoll.getNumEvents(); i++)
		{
			eventFD = events[i].data.fd;
			if (events[i].events & EPOLLIN
				&& _In.find(eventFD) != _In.end())
			{
				_In[eventFD]->input(eventFD);
				// events[i].events &= ~EPOLLIN;
				// _epoll.modifyFd(eventFD, events[i].events);
				continue;
			}
			if (events[i].events & EPOLLOUT
				&& _Out.find(eventFD) != _Out.end())
			{
				_Out[eventFD]->output(eventFD);
				// events[i].events &= ~EPOLLOUT;
				// _epoll.modifyFd(eventFD, events[i].events);
				continue;
			}
			Log::getInstance().print("Removing unknown event...\n");
			_epoll.removeFd(eventFD);
		}
    }
}

void Restaurant::addFdHandler(int fd, FdHandler* fdhandler, uint32_t eventType)
{
	if (eventType & EPOLLIN)
	{
		if (_In.find(fd) != _In.end())
			throw WebservException("FD " + std::to_string(fd) + " already exists");
		_In[fd] = fdhandler;
	}
	if (eventType & EPOLLOUT)
	{
		if (_Out.find(fd) != _Out.end())
			throw WebservException("FD " + std::to_string(fd) + " already exists");
		_Out[fd] = fdhandler;
	}
	_epoll.addFd(fd, eventType);
}

void Restaurant::removeFdHander(int fd)
{
	_Out.erase(fd);
	_In.erase(fd);
	_epoll.removeFd(fd);
}