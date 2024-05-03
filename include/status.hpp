/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 14:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/03 14:50:01 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# include <string>
# include <utility>

class Status
{
	public:
		typedef enum
		{
			OK, //200
			BAD, //400
			FORBIDDEN, //403
			NOTFOUND, //404
			NOTALLOWED, //405
			LENGTHREQUIRED, //411
			TOOLARGE, //413
			UNSUPPORTED, //415
			INTERNALERR, //500
			COUNT
		} 	key;
		
		Status();
		~Status() = default;
		
		key				_state;

		std::string&	getStatMessage();
		short&			getStatNum();

	private:
		std::pair<std::string, short> _status[COUNT];
};

#endif