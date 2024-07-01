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
#include <fcntl.h>

extern volatile sig_atomic_t interrupted;

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
//	for (auto in : _In)	{
//		delete in.second;
//	}
//	for (auto out : _Out) {
//		delete out.second;
//	}
//	_In.clear();
	_fds.clear();
}

bool Restaurant::isFdType(int fd, FdHandler::fdHandlerType type) {
	if (this->_fds.find(fd) != this->_fds.end() && this->_fds[fd]->type == type) {
		return true;
	}
	return false;
}

void Restaurant::run()
{
    epoll_event events[CLI_LIMIT];
	int			eventFD;
    while (!interrupted)
	{
		checkTimeoutsAndKickLingeringCustomers();
        _concierge.wait(1000, events);
		for (int i = 0; i < _concierge.getNumEvents(); i++)
		{
			eventFD = events[i].data.fd;
			fcntl(eventFD, F_GETFL, 0);
			if (events[i].events & EPOLLIN && _fds.find(eventFD) != _fds.end()) {
				_fds[eventFD]->input(eventFD);
			}
			else if (events[i].events & EPOLLOUT && _fds.find(eventFD) != _fds.end()) {
				_fds[eventFD]->output(eventFD);
			}
			else if (events[i].events & EPOLLHUP && this->isFdType(eventFD, FdHandler::CGITYPE)) {
				_fds[eventFD]->handleHangup();
			}
			else {
				Log::getInstance().printErr("Removing unknown event...\n");
				_concierge.removeFd(eventFD);
				close(eventFD);
			}
		}
    }
}

void Restaurant::addFdHandler(int fd, FdHandler* fdhandler, uint32_t eventType)
{
	if (_fds.find(fd) != _fds.end()) {
		throw WebservException("FD " + std::to_string(fd) + " already exists");
	}
	_fds[fd] = fdhandler;
//	if (eventType & EPOLLIN)
//	{
//		if (_In.find(fd) != _In.end())
//			throw WebservException("FD " + std::to_string(fd) + " already exists");
//		_In[fd] = fdhandler;
//	}
//	if (eventType & EPOLLOUT)
//	{
//		if (_Out.find(fd) != _Out.end())
//			throw WebservException("FD " + std::to_string(fd) + " already exists");
//		_Out[fd] = fdhandler;
//	}
	_concierge.addFd(fd, eventType);
}

void Restaurant::removeFdHandler(int fd)
{
//	_Out.erase(fd);
	_fds.erase(fd);
	_concierge.removeFd(fd);
}

void Restaurant::checkTimeoutsAndKickLingeringCustomers() {
	for (auto fdHandler : this->_waiters) {
		Waiter*	waiter = dynamic_cast<Waiter*>(fdHandler);
		if (waiter == nullptr) {
			continue ;
		}
		waiter->timeCheck();
	}
}
