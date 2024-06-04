/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Menu.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.      +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 10:35:05 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/05/21 17:47:46 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//config stuff goes here

#ifndef MENU_HPP
#define MENU_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <functional>
#include "waiter.hpp"

class Menu {

public:

	enum state {
		NO_STATE,
		SERVER_STATE,
		LOCATION_STATE
	};

	explicit Menu(std::string);
	~Menu();

	void							lex(std::string const& filename);
	void							parse();
	state							getState() const;
	std::deque<std::string>&		getTokens();
	Cook*							getCurrentCook();
	Recipe*							getCurrentRecipe();
//	std::vector<Cook>&				getSettings();
	void							setTokens(std::deque<std::string> tokens);
	void							setState(state newState);
	void							setCurrentCook(Cook*);
	void							setCurrentRecipe(Recipe*);
	bool							isDirectiveInRightContext(std::string const& directive) const;


	// error checking functions
	void			validateDirectiveSyntax(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);
	void			validateArgumentNumber(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);
	void			validateClosingToken(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);
	void			validateErrorCode(std::string const& errorCode);
	static void		validateClientBodySize(std::stringstream const& streamBodySize);
	static void		validateMethod(std::string const& method, short methodBit, short allowedMethodsBits);
	void			validateRedirectCode(std::string const& redirectCode);


	// functions handling directives
	void	serverDirective();
	void	serverNameDirective();
	void	listenDirective();
	void	locationDirective();
	void	indexDirective();
	void	errorPageDirective();
	void	autoIndexDirective();
	void	clientMaxBodySizeDirective();
	void	returnDirective();
	void	rootDirective();
	void	allowedMethodsDirective();
	void	closeBracketDirective();

	// helper functions for directives
	int				getArrayIndex(std::string const& errorCode);
	std::string		popFrontToken();
	static short	identifyMethod(std::string const& method);
	static void		splitAndInitialiseIpAndPortStrings(std::string const& ipAndPort, std::string& newIp, std::string& newPort);
	void			setNewIp(std::string const& newIp);
	void			setNewPort(std::string const& newPort);

private:


	std::vector<Kitchen>			_kitchens;
	std::deque<std::string>			_tokens;
	state							_currentState;
	Cook*							_currentCook;
	Recipe*							_currentRecipe;
	std::array<std::string, COUNT>	_errorCodesArray = {"200", "301", "302", "400", "403", "404", "405", "411", "415", "500"};

};

#endif
