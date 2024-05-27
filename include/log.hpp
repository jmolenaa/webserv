/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   log.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 13:04:50 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 12:32:38 by dliu          ########   odam.nl         */
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
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		//gets the static Log instance (singleton)
		static Log& getInstance()
		{
			static Log log;
			return log;
		}

		/**
		 * Enables or disables Logging. Default is false.
		 * Only needs to be enabled once at start of program, or can be left out entirely.
		*/
		void	enableLog(bool enabled)
		{
			_logEnabled = enabled;
			print("Logging is enabled");
		}

		//check whether logging is enabled or disabled
		bool	isEnabled() const
		{
			return (_logEnabled);
		}

		//prints the message if logging is enabled.
		void	print(const std::string& message)
		{
			if (_logEnabled)
				std::cout << "Log: " << message << std::endl;
		};
};

#endif