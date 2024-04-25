/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:06:39 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 17:14:08 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <sys/epoll.h>
# include <iostream>
# include <cstring>
# include <unistd.h>

# define CLI_LIMIT 10
# define BUF_LIMIT 1024
# define PORT 8080

typedef enum
{
	HTML,
	ERROR
} filetype;

#endif