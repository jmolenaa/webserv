/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 14:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 10:27:59 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# include <string>
# include "defines.hpp"

class Status
{
	public:
		Status();
		~Status() = default;
		
		std::string&	getStatMessage();
		short&			getStatNum();
		
		status			getState();
		void			updateState(status status);
	
	private:
		std::pair<std::string, short>	_status[COUNT];
		status							_state;
};

#endif