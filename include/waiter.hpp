/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 13:55:46 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAITER_HPP
# define WAITER_HPP

# include <sys/socket.h>
# include <netinet/in.h>

# include "defines.hpp"
# include "fdHandler.hpp"
# include "status.hpp"
# include "order.hpp"
# include "cook.hpp"
# include "dish.hpp"

class Waiter : public FdHandler
{
	public:
		Waiter(Kitchen kitch, void* rest);
		Waiter() = delete;
		~Waiter();

		void	input(int eventFD) override;
		void	output(int eventFD) override;

		void	prepOrder(int orderFD);
		void	finishOrder(int orderFD);

		const Kitchen	kitchen;

	private:
		std::unordered_map<int, Order*> _orders;
		std::unordered_map<int, Dish*>	_dishes;
};

#endif