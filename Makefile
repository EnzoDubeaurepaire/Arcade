##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME	=	arcade

CORE_SRC	=	src/main.cpp \
				src/Loader/Loader.cpp

OBJ	=	$(CORE_SRC:.cpp=.o)

GAME_LIBS	=	

DISPLAY_LIBS	=

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-fno-gnu-unique -std=c++20 -ldl
CXXFLAGS	+=	-I ./src/Loader -I ./src/Games -I ./src/DisplayModules -I ./src/Core

CC	=	g++

all: $(NAME)

$(NAME):	games graphicals core

games:
	$(foreach lib, $(GAME_LIBS), make -C $(lib);)

graphicals:
	$(foreach lib, $(DISPLAY_LIBS), make -C $(lib);)

core:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:	clean_libs clean_core

clean_libs:
	$(foreach lib, $(GAME_LIBS), make -C $(lib) clean;)
	$(foreach lib, $(DISPLAY_LIBS), make -C $(lib) clean;)

clean_core:
	rm -rf $(OBJ)

fclean:	fclean_libs fclean_core

fclean_libs:
	$(foreach lib, $(GAME_LIBS), make -C $(lib) fclean;)
	$(foreach lib, $(DISPLAY_LIBS), make -C $(lib) fclean;)

fclean_core:	clean_core
	rm -rf $(NAME)

re:	fclean all

tests_run:
	echo "No Tests"
