/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   log.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 13:04:50 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 13:52:43 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

# include <iostream>

class Log
{
	private:
		bool	_logEnabled;
		Log() : _logEnabled(false) {};

	public:
		static Log& getInstance()
		{
			static Log log;
			return log;
		}
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		void	enableLog(bool enabled)
		{
			_logEnabled = enabled;
		}

		bool	isEnabled() const
		{
			return (_logEnabled);
		}

		void	print(const std::string& message)
		{
			if (_logEnabled)
				std::cout << "Log: " << message << std::endl;
		};
};

#endif