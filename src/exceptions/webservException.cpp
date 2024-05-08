/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webservException.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/07 10:52:43 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/08 15:42:45 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by jmolenaa on 7-5-24.
//

#include "webservException.hpp"

WebservException::WebservException(std::string errorStr) : _errorStr(std::move(errorStr)){
}