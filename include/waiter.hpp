/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/18 16:50:53 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAITER_HPP
# define WAITER_HPP

# include <sys/socket.h>
# include <netinet/in.h>

# include "defines.hpp"
# include "fdHandler.hpp"
# include "status.hpp"
# include "customer.hpp"
# include "cook.hpp"

class Waiter : public FdHandler
{
	public:
		Waiter(Kitchen kitch, void* rest);
		Waiter() = delete;
		~Waiter();

		void	input(int eventFD) override;
		void	output(int eventFD) override;

		const Kitchen	kitchen;

	private:
		std::unordered_map<int, Customer*> _customers;
};

#endif