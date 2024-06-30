/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/27 18:08:39 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WAITER_HPP
# define WAITER_HPP

# include <sys/socket.h>
# include <netinet/in.h>

# include "defines.hpp"
# include "fdHandler.hpp"
# include "cook.hpp"

class Waiter : public FdHandler
{
	public:
		Waiter(Kitchen kitch, Restaurant& rest);
		Waiter() = delete;
		~Waiter() override;

		void	input(int eventFD) override;
		void	output(int eventFD) override;
		void	timeCheck();

		const Kitchen	kitchen;
		void	kickCustomer(int customerFd);
	
	private:
		std::unordered_map<int, FdHandler*> _customers;
};

#endif