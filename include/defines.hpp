/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   defines.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/26 17:17:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/07/01 13:06:45 by jmolenaa      ########   odam.nl         */
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
# define CLI_LIMIT 100
# define BUF_LIMIT 10000
# define MAX_BODY_SIZE 1000000
# define TIMEOUT 10
# define MAX_TIMEOUT 60

typedef enum {
	GET = 1,
	POST = 2,
	DELETE = 4,
	OTHER = 8
} 	method;

/**
 * NB!!! If adding to this enum:
 * remember to update _errorPaths in Recipe constructor
 * and status/status.cpp constructor
*/
typedef enum
{
	OK, //200
	MOVEDPERMANENTLY, //301
	FOUND, //302
	TEMPORARYREDIRECT ,// 307
	PERMANENTREDIRECT ,// 308
	BAD, //400
	FORBIDDEN, //403
	NOTFOUND, //404
	METHODNOTALLOWED, //405
	LENGTHREQUIRED, //411
	TOOLARGE, //413
	UNSUPPORTED, //415
	TEAPOT, //418
	INTERNALERR, //500
	LOOPDETECTED, // 508
	COUNT
} 	status;

#endif
