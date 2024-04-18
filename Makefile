# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/04/18 16:24:31 by dliu          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
# CLIENT = client
CC = c++
FLAG = -Wall -Werror -Wextra
VPATH = src src/networking
SERVER = main.cpp server.cpp epoll.cpp
# CLIENT_SRC = test_client.cpp
INCLUDE = -I include

all: $(NAME)

# client: $(CLIENT)

$(NAME): $(SERVER)
	$(CC) $(FLAG) $(INCLUDE) $^ -o $(NAME)

# $(CLIENT): $(CLIENT_SRC)
# 	$(CC) $(FLAG) $(INCLUDE) $^ -o $(CLIENT)

clean:
	rm -rf $(NAME)

fclean:clean

re: clean all

.PHONY: all clean
