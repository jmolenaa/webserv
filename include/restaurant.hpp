/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   restaurant.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:06:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:04:10 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESTAURANT_HPP
# define RESTAURANT_HPP

#include <vector>
#include <unordered_map>

#include "epoll.hpp"
#include "waiter.hpp"

class Restaurant
{
	public:
		explicit Restaurant(std::string const& filename);
		Restaurant() = delete;
		Restaurant(Restaurant& other) = delete;
		~Restaurant();
		
		void	run();
		void	addFdHandler(int fd, FdHandler* fdhandler, uint32_t eventType);
		void	removeFdHander(int fd);

	private:
		Epoll	_epoll;
	
		std::vector<Waiter*>	_waiters;

		std::unordered_map<int, FdHandler*>	_In;
		std::unordered_map<int, FdHandler*>	_Out;

		void	_handleInput(int eventFD);
		void	_handleOutput(int eventFD);
};

#endif