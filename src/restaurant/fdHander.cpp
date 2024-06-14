/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fdHander.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/14 11:59:59 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 12:08:55 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "fdHandler.hpp"
#include "restaurant.hpp"

FdHandler::FdHandler(void* restaurantPointer): resP(restaurantPointer), _inFD(-1), _outFD(-1) {}
	
FdHandler::~FdHandler()
{
	Restaurant* restaurant = (Restaurant*) resP;
	if (_inFD > 0)
		restaurant->removeFdHander(_inFD);
	if (_outFD > 0)
		restaurant->removeFdHander(_outFD);
}

int	FdHandler::getIn() {return _inFD;}

int FdHandler::getOut() {return _outFD;}