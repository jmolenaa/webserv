# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/04/16 12:46:37 by yizhang       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CLIENT = client
CC = c++
FLAG = -Wall -Werror -Wextra
SERVER = server.cpp
CLIENT_SRC = client.cpp

all: $(NAME)

client: $(CLIENT)

$(NAME): $(SERVER)
	$(CC) $(FLAG) $(SERVER) -o $(NAME)

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(FLAG) $(CLIENT_SRC) -o $(CLIENT)


clean:
	rm -rf $(NAME)

fclean:clean

re: clean all

.PHONY: all clean
