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
	if (menu.getKitchens().empty()) {
		throw WebservException("Webserv: configuration file: No servers added\n");
	}
	for (Kitchen const& kitchen : menu.getKitchens()) {
		this->_waiters.push_back(new Waiter(kitchen, *this));
	}
}

Restaurant::~Restaurant()
{
	for (auto waiter : _waiters) {
		delete waiter;
	}
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
	_concierge.addFd(fd, eventType);
}

void Restaurant::removeFdHandler(int fd)
{
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
