# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/04/23 19:09:07 by dliu          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CC = c++
FLAG = -Wall -Werror -Wextra
VPATH = src src/server src/helpers
SERVER = main.cpp \
			helpers.cpp \
			server.cpp _setup.cpp _serverHandlers.cpp _clientHandlers.cpp \
		 
INCLUDE = -I include

all: $(NAME)

$(NAME): $(SERVER)
	$(CC) $(FLAG) $(INCLUDE) $^ -o $(NAME)


clean:
	rm -rf $(NAME)

fclean:clean

re: clean all

.PHONY: all clean
