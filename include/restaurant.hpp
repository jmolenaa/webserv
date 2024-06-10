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
#include "waiter.hpp"

class	Restaurant {

public:
	explicit Restaurant(std::string const& filename);
	Restaurant() = delete;
	Restaurant(Restaurant& other) = delete;
	~Restaurant() = default;
	
	void	run();
	void	oldInit(); // remove later, kept just in case

private:
	void					_initialiseWaiters(std::string const& filename);
	Epoll 					_epoll;
	std::vector<Waiter*>	_waiters;
};

#endif