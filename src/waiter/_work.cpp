/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _work.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:25 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/12 15:06:49 by dliu          ########   odam.nl         */
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
 * @todo needs to go through epoll
 * move this to be inside Order class? Or make a new class for handling all reading and writing?
 * First read the request header BUF_LIMIT at a time until it finds "\r\n"
 * Then parse the content based on the headers CONTENT_LENGTH, BUF_LIMIT at a time (if it's smaller than limit)
*/
void Waiter::_takeOrder(int fd)
{
	char buffer[2048];
	ssize_t numBytes = read(fd, buffer, sizeof(buffer) - 1);
	buffer[numBytes] = '\0';
	std::string orderString(buffer);
	if (numBytes < 0)
	{
		close(fd);
		orderString = "";
	}
	
	Log::getInstance().print("\nGOT REQUEST:\n" + orderString);

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

/**
 * @todo needs to go through epoll
 * send BUF_LIMIT at a time
*/
void Waiter::_serveCustomer(int customerFd, const std::string& meal)
{
	Log::getInstance().print("=====SEND=====\n" + meal + "\n=====END=====\n");
	
	ssize_t bytesSent = send(customerFd, meal.c_str(), meal.size(), 0);
	close(customerFd);
	if (bytesSent == -1)
		throw (WebservException("Failed to send dish: " + meal + "\n"));
}
