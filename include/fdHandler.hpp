/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fdhandler.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/13 14:14:42 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/13 17:59:51 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDHANDLER_HPP
# define FDHANDLER_HPP

#include <string>

class FdHandler
{
	public:
		void*	resP;

		FdHandler(void* restaurantPointer);
		virtual ~FdHandler();

		virtual status input(int eventFD) = 0;
		virtual status output(int eventFD) = 0;

		int	getIn();
		int getOut();
	
	protected:
		int	_inFD;
		int _outFD;

};

#endif