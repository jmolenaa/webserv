/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/12 15:08:59 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "waiter.hpp"
#include "log.hpp"

Waiter::Waiter(Epoll& epoll, Kitchen kitchen)
	: _epoll(epoll), _kitchen(kitchen)
{
	Log::getInstance().print("Waiter has been hired");

	_createSocket();
	_bindToAddress();
    _epoll.addFd(_waiterFd, EPOLLIN);

	if (listen(_waiterFd, SOMAXCONN) == -1)
		throw WebservException("Waiter could not listen because: " + std::string(std::strerror(errno)) + "\n");
}

Waiter::~Waiter()
{
    if (_waiterFd > 0)
        close(_waiterFd);
}

/**
 * @todo epoll needs to keep checking FDs until they are done reading / have been closed
 * Consider making a class for reading and writing
 * 
 * Places where reading/writing will take place:
 * Read the request from client
 * Read the file from root
 * CGIs read and write through pipes
 * Write the response to the client
*/
void Waiter::work()
{
	Log::getInstance().print("Waiter is working with " + std::to_string(_kitchen.size()) + " Cooks in the kitchen");
    epoll_event events[CLI_LIMIT];
    while (true)
	{
        _epoll.wait_events(-1, events);
		for (int i = 0; i < _epoll.getNumEvents(); i++)
		{
			if (events[i].data.fd == _waiterFd)
				_welcomeCustomer();
			else
				_takeOrder(events[i].data.fd);
		}
    }
}
