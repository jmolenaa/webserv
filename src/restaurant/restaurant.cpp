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

#include "log.hpp"
#include "restaurant.hpp"
#include "menu.hpp"
#include "cook.hpp"
#include <csignal>

void signal_handler(int signal_num) 
{ 
    std::cout << "The interrupt signal is (" << signal_num 
         << "). \n"; 
  
    // It terminates the  program 
    exit(signal_num); 
} 

Restaurant::Restaurant(std::string const& filename)
{
	Log::getInstance().print("Constructing Restaurant instance");
	Log::getInstance().print("Parsing '" + filename);

	Menu	menu(filename);
	menu.parse();
	for (Kitchen const& kitchen : menu.getKitchens()) {
		this->_waiters.push_back(new Waiter(kitchen, *this));
	}
}

Restaurant::~Restaurant()
{
	for (auto waiter : _waiters) {
		delete waiter;
	}
	for (auto in : _In)	{
		delete in.second;
	}
	for (auto out : _Out) {
		delete out.second;
	}
	_In.clear();
	_Out.clear();
}

void Restaurant::run()
{
    epoll_event events[CLI_LIMIT];
	int			eventFD;
    while (true)
	{
        _concierge.wait(-1, events);
		for (int i = 0; i < _concierge.getNumEvents(); i++)
		{
			eventFD = events[i].data.fd;
			if (events[i].events & EPOLLIN
				&& _In.find(eventFD) != _In.end())
			{
				// if (_concierge.set_non_blocking(eventFD) == -1) 
				// {
				// 		close(eventFD);
				// 		continue;
				// }
				_In[eventFD]->input(eventFD);
				// if (events[i].events & EPOLLHUP) 
				// {
				// 	// Handle hang-up
				// 	int client_fd = events[i].data.fd;
				// 	std::cout << "Hang-up detected, closing connection" << std::endl;
				// 	close(client_fd);
             	// }
				//std::cout<<"epollin"<<std::endl;
				//events[i].events = EPOLLOUT;
				//_concierge.modifyFd(eventFD, events[i].events);
				continue;
			}
			else if (events[i].events & EPOLLOUT
				&& _Out.find(eventFD) != _Out.end())
			{
				_Out[eventFD]->output(eventFD);
				// std::cout<<"epollout"<<std::endl;
				// events[i].events &= ~EPOLLOUT;
				// _epoll.modifyFd(eventFD, events[i].events);

				continue;
			}
			else{
				Log::getInstance().print("Removing unknown event...\n");
				_concierge.removeFd(eventFD);
				//close(eventFD);
			}
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
	_concierge.addFd(fd, eventType);
}

void Restaurant::removeFdHandler(int fd)
{
	_Out.erase(fd);
	_In.erase(fd);
	_concierge.removeFd(fd);
}