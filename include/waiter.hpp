/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/13 20:05:22 by dliu          ########   odam.nl         */
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

class Waiter : public FdHandler
{
	public:
		Waiter(Kitchen kitch, void* rest);
		Waiter() = delete;
		~Waiter();

		status	input(int eventFD) override;
		status	output(int eventFD) override;

		const Kitchen	kitchen;
		void*			restaurant;

		void	finishOrder(int orderFD);

	private:
		int									_waiterFD;	
		std::unordered_map<int, Order*> 	_orders;
};

#endif