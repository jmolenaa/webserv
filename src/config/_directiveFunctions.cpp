/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _directiveFunctions.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/17 11:50:26 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/17 11:50:26 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "menu.hpp"
#include <iostream>
#include "webservException.hpp"

void Menu::serverDirective() {

	this->validateDirectiveSyntax("server", 1, "{");
	this->setState(SERVER_STATE);
	this->setCurrentCook(new Cook());
	this->setCurrentRecipe(new Recipe());

}

void Menu::serverNameDirective() {
	this->validateDirectiveSyntax("server_name", 2, ";");

	std::string	token = this->getTokens().front();
	std::cout << "servername" << std::endl;
	this->getTokens().pop_front();
}

void Menu::listenDirective() {
	std::cout << "listen" << std::endl;
	this->getTokens().pop_front();

}

void Menu::locationDirective() {
	std::cout << "location" << std::endl;
	this->getTokens().pop_front();
}

void Menu::indexDirective() {
	std::cout << "index" << std::endl;
	this->getTokens().pop_front();
}

void Menu::errorPageDirective() {
	std::cout << "errorpage" << std::endl;
	this->getTokens().pop_front();
	this->getTokens().pop_front();

}

void Menu::autoindexDirective() {
	std::cout << "autoindex" << std::endl;
	this->getTokens().pop_front();

}

void Menu::clientMaxBodySizeDirective() {
	std::cout << "clinetmaxbody" << std::endl;
	this->getTokens().pop_front();

}

void Menu::returnDirective() {
	std::cout << "return" << std::endl;
	this->getTokens().pop_front();
	this->getTokens().pop_front();

}

void Menu::rootDirective() {
	std::cout << "root" << std::endl;
	this->getTokens().pop_front();

}

void Menu::allowedMethodsDirective() {
	this->getTokens().pop_front();

}

void Menu::closeBracketDirective() {

}

