/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   configParser.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 10:35:05 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/05/21 17:47:46 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//config stuff goes here

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <functional>
#include "serverConfig.hpp"
#include "lexer.hpp"

//typedef void	(*directiveFunc)();

class ConfigParser {

public:

	enum state {
		NO_STATE,
		SERVER_STATE,
		LOCATION_STATE
	};

	ConfigParser();
	~ConfigParser() = default;

	void						lex(std::string const& filename);
	void						parse();
	state						getState() const;
	std::deque<std::string>&	getTokens();
	void						setTokens(std::deque<std::string> tokens);

	// functions handling directives
//	void	setupMap(std::unordered_map<std::string, std::function<void(ConfigParser&)>>&);
	void	serverDirective();

private:


	std::vector<ServerConfig>	_serverSettings;
	std::deque<std::string>		_tokens;
	state						_currentState;

};

#endif
