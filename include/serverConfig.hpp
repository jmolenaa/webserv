/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverConfig.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:38 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/22 14:58:49 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

# include <string>
# include <unordered_map>
# include "location.hpp"

class ServerConfig {
	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other) = default;

		uint16_t		getPort() const;
		uint32_t		getAddress() const;
		std::string		getName() const;
		Location		matchLocation(std::string path) const;
	
	private:
		uint16_t									_port;
		uint32_t									_address;
		std::string									_name;
		std::unordered_map<std::string, Location>	_locations;
};


#endif
