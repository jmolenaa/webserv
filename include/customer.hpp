/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   customer.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/18 13:45:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/18 17:01:41 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOMER_HPP
# define CUSTOMER_HPP

# include "defines.hpp"
# include "fdHandler.hpp"
# include "order.hpp"
# include "dish.hpp"

class Customer : public FdHandler
{
	public:
		Customer(int fd, void* restaurantPointer, void* waiterPointer);
		Customer() = delete;
		Customer(Customer& other) = delete;
		~Customer();

		void	input(int eventFD) override;
		void	output(int eventFD) override;
	
		void	eat();

	private:
		void*	_wP;
		Status	_status;
		Order	_order;
		Dish*	_dish;

		std::string _food;
		ssize_t		_bitesLeft;
		ssize_t		_pos;

		void	_getDish();
		void	_leave();
};

#endif