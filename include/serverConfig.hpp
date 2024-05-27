/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverConfig.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:38 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/27 12:45:33 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

# include <string>
# include <unordered_map>
# include "location.hpp"

class ServerConfig {
	public:
		explicit	ServerConfig();
		explicit	ServerConfig(uint16_t port, uint32_t address, std::string& name, Locations& locations);
		explicit	ServerConfig(const ServerConfig& other);
		ServerConfig& operator=(const ServerConfig& other);

		std::string		getName() const;
		uint16_t		getPort() const;
		uint32_t		getAddress() const;
		Locations		getLocations() const;

		Location		matchLocation(std::string path) const;
	
	private:
		uint16_t	_port;
		uint32_t	_address;
		std::string	_name;
		Locations	_locations;
};

typedef std::unordered_map<std::string, ServerConfig> ServerConfigs;

#endif
