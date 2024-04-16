/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 10:26:42 by yizhang       #+#    #+#                 */
/*   Updated: 2024/04/16 11:19:43 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        std::cout<<"get config:"<<argv[1]<<std::endl;
    }
    else
    {
        std::cout<<"Invalid number of arguments."<<std::endl;
        return 1;
    }
    return 0;
}