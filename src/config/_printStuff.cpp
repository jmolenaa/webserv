/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _printStuff.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/10 12:25:06 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/10 12:25:06 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "menu.hpp"
#include "status.hpp"
#include <arpa/inet.h>

void	printErrorPaths(std::array<std::string, COUNT> const& errorPaths) {
	Status	errorCode;
	int i = 1;

	errorCode.updateState(OK);
	for (std::string error : errorPaths) {
		std::cout << errorCode.getStatNum() << " " << error << "\n";
		errorCode.updateState((status)i);
		i++;
	}
}

void	printMethods(short methods) {
	if (methods == 0) {
		std::cout << "NONE";
	}
	if ((methods & GET) != 0) {
		std::cout << "GET,";
	}
	if ((methods & POST) != 0) {
		std::cout << "POST,";
	}
	if ((methods & DELETE) != 0) {
		std::cout << "DElETE,";
	}
	std::cout << "\n";
}

void	printRecipe(Recipe const& recipe) {
	Status	redirectionStatus;

	std::cout << "name: " << recipe.page << "\n";
	std::cout << "root: " << recipe.root << "\n";
	std::cout << "index: " << recipe.index << "\n";
	std::cout << "allowed_methods: ";
	printMethods(recipe.allowedMethods);
	std::cout << "client body size: " << recipe.maxBodySize << "\n";
	std::cout << "autoindex: " << std::boolalpha << recipe.autoindex << "\n";
	redirectionStatus.updateState(recipe.redir.first);
	std::cout << "redirection: " << redirectionStatus.getStatNum() << " with url " << recipe.redir.second << "\n";
	std::cout << "error paths:\n";
	printErrorPaths(recipe.errorPaths);
	std::cout << "\n";
}

void	printLocations(Cookbook cookbook) {
	std::cout << "--------Default location:----------\n";
	printRecipe(cookbook["root"]);
	for (auto recipe : cookbook) {
		if (recipe.first != "root") {
			std::cout << "-------Next location-------\n";
			printRecipe(recipe.second);
		}
	}
}

void	printAddress(uint32_t address) {
	char str[INET_ADDRSTRLEN];
	struct in_addr	ipStruct;
	ipStruct.s_addr = address;
	inet_ntop(AF_INET, &ipStruct, str, INET_ADDRSTRLEN);
	std::cout << str;
}

void	printKitchen(Kitchen const& kitchen) {
	std::cout << BLUE << "\n----------New kitchen-----------\n";
	std::cout << "Server listening on host ";
	printAddress(kitchen.begin()->second.getAddress());
	std::cout << " and port " << ntohs(kitchen.begin()->second.getTable()) << "\n";
	std::cout << "Default server being " << kitchen.begin()->second.getName() << "\n" << RESET;
	for (auto& it : kitchen) {
		Cook cookie;
		cookie = it.second;
		std::cout << GREEN << "\n<<<<<<<Next server>>>>>>>\n" << RESET;
		std::cout << "\nServer name: " << cookie.getName();
		std::cout << "\nWith locations: \n\n";
		printLocations(cookie.getCookbook());
	}
}


void Menu::printStuff() {
	for (Kitchen kitchen : this->getKitchens()) {
		printKitchen(kitchen);
	}
}
