/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _work.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:25 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/11 17:58:33 by dliu          ########   odam.nl         */
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

/**
 * @todo fix this shit
*/
void Waiter::_takeOrder(int fd)
{
	//if order is larger than buffer size, this needs to loop
	char buffer[BUF_LIMIT];
	ssize_t numBytes = read(fd, buffer, sizeof(buffer) - 1);
	buffer[numBytes] = '\0';
	std::string orderString(buffer);

	// while (numBytes > 0)
	// {
	// 	orderString += std::string(buffer);
	// 	numBytes = read(fd, buffer, sizeof(buffer) - 1);
	// 	buffer[numBytes] = '\0';
	// }
	if (numBytes < 0)
	{
		close(fd);
		orderString = "";
	}
	
	Order order(orderString);
	const Cook* cook = _kitchen.find(order.getCookName()); //default cook
	if (cook == nullptr)
		cook = _kitchen.begin();

	std::string page = order.getPath();
	Recipe recipe(cook->getRecipe(page));
	while (!page.empty() && page != recipe.page) //double check this shit
	{
		// std::cout << page << std::endl; //DEEBUGGING
		size_t end = page.find_last_of('/');
		if (end == std::string::npos)
			break;
		else
			page = page.substr(0, end);
		recipe = cook->getRecipe(page);
	}
	Dish dish(_epoll, order, recipe);
	_serveCustomer(fd, dish.getMeal());
}

void Waiter::_serveCustomer(int customerFd, const std::string& meal)
{
	Log::getInstance().print("=====SEND=====\n" + meal + "\n=====END=====\n");
	
	ssize_t bytesSent = send(customerFd, meal.c_str(), meal.size(), 0);
	close(customerFd);
	if (bytesSent == -1)
		throw (WebservException("Failed to send dish: " + meal + "\n"));
}
