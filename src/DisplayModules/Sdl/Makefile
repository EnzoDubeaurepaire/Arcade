##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile for SDL display module
##

NAME	=	../../../lib/arcade_sdl2.so

SRC	=	SdlModule.cpp \
		SdlWrapper.cpp \

OBJ	=	$(SRC:.cpp=.o)

CXXFLAGS	+=	-Wall -Wextra -Werror
CXXFLAGS	+=	-fPIC -std=c++20
CXXFLAGS	+=	-I../../../src/Core
CXXFLAGS	+=	-I../../../src/DisplayModules
CXXFLAGS	+=	-I../../../src/Core/Objects

LDFLAGS	+=	-shared
LDFLAGS	+=	-lSDL2 -lSDL2_ttf -lSDL2_image

CC	=	g++

all:	$(NAME)

$(NAME):	$(OBJ)
	mkdir -p ../../../lib
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
