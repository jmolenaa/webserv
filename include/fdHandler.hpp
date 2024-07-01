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
class Restaurant;

class FdHandler
{
	public:

		enum fdHandlerType {
			WAITERTYPE,
			CUSTOMERTYPE,
			DISHTYPE,
			CGITYPE
		};

		FdHandler(Restaurant& rest, fdHandlerType type) : restaurant(rest), type(type), _inFD(-1), _outFD(-1) {};
		virtual ~FdHandler() = default;

		virtual void input(int eventFD) = 0;
		virtual void output(int eventFD) = 0;

		virtual void handleHangup() {};

		Restaurant&	restaurant;


		fdHandlerType	type;

	protected:
		int	_inFD;
		int _outFD;
};

#endif