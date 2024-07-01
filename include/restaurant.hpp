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

#include "concierge.hpp"
#include "fdHandler.hpp"


class Restaurant
{
	public:
		explicit Restaurant(std::string const& filename);
		Restaurant() = delete;
		Restaurant(Restaurant& other) = delete;
		~Restaurant();
		
		void	run();

		void	addFdHandler(int fd, FdHandler* handler, uint32_t eventType);
		void	removeFdHandler(int fd);
		bool	isFdType(int fd, FdHandler::fdHandlerType type);
		void	checkTimeoutsAndKickLingeringCustomers();

	private:
		Concierge					_concierge;
		std::vector<FdHandler*>		_waiters;
		
		std::unordered_map<int, FdHandler*>		_fds;
		std::unordered_map<int, FdHandler*>		_Out;
};

#endif