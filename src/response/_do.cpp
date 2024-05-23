#include "response.hpp"
#include <iostream>

void Response::_doMethod(method meth, Request& request)
{
	switch (meth)
	{
		case (GET):
			_get();
			break;
		case (POST):
			_post(request);
			break;
		case (DELETE):
			_post(request);
			break;
		default:
			_status.updateState(METHODNOTALLOWED);
			break;
	}
}

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

void Response::_executeCGI()
{
	//To be done
	_body += "CGI STUFF NOT YET SUPPORTED";
}