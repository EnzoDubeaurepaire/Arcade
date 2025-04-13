##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile for Minesweeper
##

NAME		=	arcade_minesweeper.so

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-std=c++20 -fPIC -fno-gnu-unique
CXXFLAGS	+=	-I.. -I../../Core -I../../Core/Objects

LDFLAGS		+=	-shared
ifeq ($(DEBUG),1)
CXXFLAGS += -g3
endif

CXX		=	g++

SRC		=	Minesweeper.cpp \
			../../Core/Objects/Sprite.cpp \
			../../Core/Objects/Text.cpp

OBJ		=	$(SRC:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	mkdir -p ../../../lib/
	mv $(NAME) ../../../lib/

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -rf ../../../lib/$(NAME)

re:	fclean all

.PHONY: all clean fclean re
