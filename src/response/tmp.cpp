#include "response.hpp"
#include <iostream>

void Response::_post(Request& request)
{
	std::string body = request.getBody();
	std::cout << "POSTING:" << body << std::endl;
}

void Response::_delete(Request& request)
{
	std::string path = request.getPath();
	std::cout << "DELETING" << path << std::endl;
}