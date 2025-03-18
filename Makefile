##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME	=	arcade

CORE_SRC	=	src/main.cpp \
				src/Loader/Loader.cpp \
				src/Interfaces/IDisplayModule.cpp

OBJ	=	$(CORE_SRC:.cpp=.o)

LIBS	=

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-fno-gnu-unique -std=c++20 -ldl
CXXFLAGS	+=	-I ./src/Loader -I ./src/Interfaces

CC	=	g++

all: $(NAME)

$(NAME):	build_libs build_core

build_libs:
	$(foreach lib, $(LIBS), make -C $(lib);)

build_core:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:	clean_libs clean_core

clean_libs:
	$(foreach lib, $(LIBS), make -C $(lib) clean;)

clean_core:
	rm -rf $(OBJ)

fclean:	fclean_libs fclean_core

fclean_libs:
	$(foreach lib, $(LIBS), make -C $(lib) fclean;)

fclean_core:	clean_core
	rm -rf $(NAME)

re:	fclean all

tests_run:
	echo "No Tests"
