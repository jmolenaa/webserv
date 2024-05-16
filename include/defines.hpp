/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   defines.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/26 17:17:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/03/26 17:17:30 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define CYAN "\x1b[36m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

#define PORT 8080
#define CLI_LIMIT 10
#define BUF_LIMIT 1024
#define MAX_BODY_SIZE 100000000

typedef enum e_methods {
	GET = 1,
	POST = 2,
	DELETE = 4,
	OTHER = 8
} t_methods;

#endif

