# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/05/03 16:14:47 by dliu          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = Webserv
CC = c++
FLAG = -Wall -Werror -Wextra
VPATH = src src/server src/helpers src/request src/response src/status src/config
SERVER = main.cpp \
			helpers.cpp \
			request.cpp \
			response.cpp _body.cpp _header.cpp _errors.cpp \
			server.cpp _setup.cpp _serverHandlers.cpp _clientHandlers.cpp \
		 	status.cpp \

CONFIG = configmain.cpp config.cpp lexer.cpp

INCLUDE = -I include

all: $(NAME) config

$(NAME): $(SERVER)
	$(CC) $(FLAG) $(INCLUDE) $^ -o $(NAME)


clean:
	rm -rf $(NAME)

fclean:clean

re: clean all

.PHONY: all clean

config : $(CONFIG)
	$(CC) $(FLAG) $(INCLUDE) $^ -o $@
