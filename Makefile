# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: yizhang <yizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2024/04/16 10:28:05 by yizhang       #+#    #+#                  #
#    Updated: 2024/07/01 13:12:48 by jmolenaa      ########   odam.nl          #
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
NAME = webserv
INCLUDE = -I include
CXX = c++
ifdef DEBUG
	CXXFLAGS := -Wall -Werror -Wextra -fsanitize=address
else
	CXXFLAGS := -Wall -Werror -Wextra -g
endif


HEADERS  = include/concierge.hpp \
			include/restaurant.hpp \
			include/cook.hpp \
			include/lexer.hpp \
			include/menu.hpp \
			include/orderedMap.hpp \
			include/status.hpp \
			include/webservException.hpp \
			include/CGI.hpp \
			include/defines.hpp \
			include/dish.hpp \
			include/log.hpp \
			include/order.hpp \
			include/recipe.hpp \
			include/customer.hpp \
			include/fdHandler.hpp \
			include/waiter.hpp 
			
SRC_FILES = src/exceptions/webservException.cpp \
			src/restaurant/restaurant.cpp \
			src/concierge/concierge.cpp \
			src/main.cpp \
			src/config/_directiveUtils.cpp \
			src/config/_errorCheckingFunctions.cpp \
			src/config/lexer.cpp \
			src/config/menu.cpp \
			src/config/_directiveFunctions.cpp \
			src/status/status.cpp \
			src/cook/cook.cpp \
			src/waiter/dish/CGI.cpp \
			src/waiter/dish/_directorylisting.cpp \
			src/waiter/dish/_errorUtils.cpp \
			src/waiter/dish/_spatula.cpp \
			src/waiter/dish/_methods.cpp \
			src/waiter/dish/dish.cpp \
			src/waiter/order/_parseOrder.cpp \
			src/waiter/order/order.cpp \
			src/waiter/customer.cpp \
			src/waiter/waiter.cpp


OBJ_DIR = obj
OBJS := $(SRC_FILES:src/%.cpp=$(OBJ_DIR)/%.o)


# Variable for dependency files
DEPS := $(OBJS:%.o=%.d)

# if condition to create obj directories when compiling for the first time
ifndef $(shell find $(OBJ_DIR) -maxdepth 1 -name $(OBJ_DIR))
	DIR = obj obj/exceptions obj/restaurant obj/config obj/cook obj/concierge obj/status obj/waiter obj/waiter/dish obj/waiter/order
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

re: fclean all

# should probably fclean first before compiling with debug symbols
debug:
	$(MAKE) DEBUG=1 all

.PHONY: all clean

