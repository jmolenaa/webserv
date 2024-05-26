# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/05/24 14:06:43 by dliu          ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

RED=\033[1;31m
GREEN=\033[1;32m
YELLOW=\033[1;33m
BLUE=\033[1;34m
MAGENTA=\033[1;35m
CYAN=\033[1;36m
END=\033[0m

# Makefile
# If you created a new file, you don't have to add it anywhere
# If you created a new directory in the src folder
# Add it to the $(DIR) variable on line 46 with changing the src prefix to the obj prefix

# Compilation variables
NAME = Webserv
INCLUDE = -I include
CXX = c++
ifdef DEBUG
	CXXFLAGS := -Wall -Werror -Wextra -g -fsanitize=address
else
	CXXFLAGS := -Wall -Werror -Wextra
endif


HEADERS  = $(shell find include -type f -name "*.hpp")
SRC_FILES = $(shell find src -type f -name "*.cpp")


OBJ_DIR = obj
OBJS := $(SRC_FILES:src/%.cpp=$(OBJ_DIR)/%.o)


# Variable for dependency files
DEPS := $(OBJS:%.o=%.d)

# if condition to create obj directories when compiling for the first time
ifndef $(shell find $(OBJ_DIR) -maxdepth 1 -name $(OBJ_DIR))
	DIR = obj obj/cgi obj/config obj/epoll obj/request obj/response obj/server obj/status obj/exceptions
endif


all: $(NAME)

$(NAME): $(DIR) $(OBJS)
	@echo "${MAGENTA}Creating $@${END}"
	@$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)
	@echo "${GREEN}Done!${END}"

$(DIR):
	@mkdir -p $@


# This tells makefile to use dependency files to check if files need to update
# no idea how this works exactly, but it seems to do the trick
-include $(DEPS)

obj/%.o: src/%.cpp
	@echo "${BLUE}Compiling $<${END}"
	@$(CXX) $(CXXFLAGS) $(INCLUDE) -MMD -c $< -o $@

clean:
	@echo "${RED}Removing objs and dependency files${END}"
	@rm -rf $(OBJS) $(DEPS) $(OBJ_DIR)

fclean: clean
	@echo "${RED}Removing ${NAME}${END}"
	@rm -rf $(NAME)

re: clean all

# should probably fclean first before compiling with debug symbols
debug:
	$(MAKE) DEBUG=1 all

.PHONY: all clean

