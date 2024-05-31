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


	// syntax checking functions
	void	validateDirectiveSyntax(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);
	void	validateArgumentNumber(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);
	void	validateClosingToken(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken);

	// functions handling directives
	void	serverDirective();
	void	serverNameDirective();
	void	listenDirective();
	void	locationDirective();
	void	indexDirective();
	void	errorPageDirective();
	void	autoindexDirective();
	void	clientMaxBodySizeDirective();
	void	returnDirective();
	void	rootDirective();
	void	allowedMethodsDirective();
	void	closeBracketDirective();

private:


	std::vector<Waiter*>		_waiters;
	std::deque<std::string>		_tokens;
	state						_currentState;
	Cook*						_currentCook;
	Recipe*						_currentRecipe;

};

#endif
