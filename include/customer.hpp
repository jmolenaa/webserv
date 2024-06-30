/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   customer.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/18 13:45:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/27 14:51:01 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOMER_HPP
# define CUSTOMER_HPP

# include "defines.hpp"
# include "fdHandler.hpp"
# include "waiter.hpp"
# include "order.hpp"
# include "dish.hpp"
# include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> t_time;

class Customer : public FdHandler
{
	public:
		Customer(int fd, Restaurant& rest, Waiter& wait);
		Customer() = delete;
		Customer(Customer& other) = delete;
		~Customer() override;

		void	input(int eventFD) override;
		void	output(int eventFD) override;
		void	eat();
		t_time	getLastAction();
		bool	handleTimeout();
		void	resetTime();
		t_time	getRequestStart();
		void	resetStartTime();

	private:
		Waiter&	_waiter;
		Status	_status;
		Order	_order;
		Dish*	_dish;

		std::string _food;
		ssize_t		_bitesLeft;
		ssize_t		_pos;
		int			_customerFd;
		t_time 		_lastAction;
		t_time		_requestStart;

		void		_getDish();
};

#endif