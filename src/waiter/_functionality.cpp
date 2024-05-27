/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _functionality.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:25 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 16:37:44 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "waiter.hpp"
#include "order.hpp"
#include "dish.hpp"
#include "log.hpp"

/**
 * Accepts new connection on socket and adds to epoll instance
*/
void Waiter::_welcomeCustomer() 
{
	sockaddr_in customerAddr{};
	socklen_t customerAddrLen = sizeof(customerAddr);
	int customerFd = accept(_waiterFd, reinterpret_cast<sockaddr *>(&customerAddr), &customerAddrLen);
	if (customerFd == -1)
		throw WebservException("Failed to welcome a new customer: " + std::string(std::strerror(errno)) + "\n");
	_epoll.addFd(customerFd, EPOLLIN);
}

void Waiter::_takeOrder(int fd)
{
	char buffer[1024];
	ssize_t numBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (numBytes <= 0)
		close(fd);
	else
	{
		buffer[numBytes] = '\0';
		Order order(buffer);

		Cook& cook = _kitchen.begin()->second;
		auto it = _kitchen.find(order.getCookName());
		if (it != _kitchen.end())
			cook = it->second;

		std::string path = order.getPath();
		Recipe recipe = cook.getRecipe(path);
		while (!path.empty() && path != recipe.path) //double check this shit
		{
			// std::cout << path << std::endl; //DEEBUGGING
			size_t end = path.find_last_of('/');
			if (end == std::string::npos)
				break;
			else
				path = path.substr(0, end);
			recipe = cook.getRecipe(path);
		}
		Dish dish(_epoll, order, recipe);
		_serveCustomer(fd, dish.getMeal());
	}
}

void Waiter::_serveCustomer(int customerFd, const std::string& meal)
{
	Log::getInstance().print("=====SEND=====\n" + meal + "\n=====END=====\n");
	
	ssize_t bytesSent = send(customerFd, meal.c_str(), meal.size(), 0);
	close(customerFd);
	if (bytesSent == -1)
		throw (WebservException("Failed to send dish: " + meal + "\n"));
}
