/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   defines.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/26 17:17:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/06 17:17:07 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
# define DEFINES_HPP

# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define CYAN "\x1b[36m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define RESET "\x1b[0m"

# define PORT 8080
# define CLI_LIMIT 10
# define BUF_LIMIT 1024
# define MAX_BODY_SIZE 100000000

typedef enum {
	GET = 1,
	POST = 2,
	DELETE = 4,
	OTHER = 8
} 	method;

typedef enum
{
	NONE,
	HTML,
	FOLDER,
	CGI
}	filetype;

/**
 * NB!!! If adding to this enum:
 * remember to update _errorPaths in Recipe constructor
 * and status/status.cpp constructor
 * also add it in menu.hpp to the _errorCodesArray in the proper spot (is in the private variables part, at the bottom)
*/
typedef enum
{
	OK, //200
	PERMANENTREDIRECT, //301
	TEMPORARYREDIRECT, //302
	BAD, //400
	FORBIDDEN, //403
	NOTFOUND, //404
	METHODNOTALLOWED, //405
	LENGTHREQUIRED, //411
	// TOOLARGE, //413
	UNSUPPORTED, //415
	INTERNALERR, //500
	COUNT
} 	status;

#endif
