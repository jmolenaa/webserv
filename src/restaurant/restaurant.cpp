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
	Log::getInstance().print("Parsing '" + filename + "'...jk");

	Menu	menu(filename);

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
        _epoll.wait_events(-1, events);
		for (int i = 0; i < _epoll.getNumEvents(); i++)
		{
			eventFD = events[i].data.fd;
			if (events[i].events & EPOLLIN
				&& _In.find(eventFD) != _In.end())
			{
				_In[eventFD]->input(eventFD);
				continue;
			}
			if (events[i].events & EPOLLOUT
				&& _Out.find(eventFD) != _Out.end())
			{
				_Out[eventFD]->output(eventFD);
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
		_In[fd] = fdhandler;
	}
	if (eventType & EPOLLOUT)
	{
		_Out[fd] = fdhandler;
	}
	_epoll.addFd(fd, eventType);
}

void Restaurant::removeFdHander(int fd)
{
	_Out.erase(fd);
	_In.erase(fd);
	_epoll.removeFd(fd);
	close(fd);
}