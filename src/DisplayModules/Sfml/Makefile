##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME	=		arcade_sfml.so

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-std=c++20 -fPIC
CXXFLAGS	+=	-I ../ -I ../../Core

LDFLAGS	+=	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
LDFLAGS	+=	-shared

CXX = g++

SRC	=	SfmlModule.cpp

OBJ	=	$(SRC:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	mv $(NAME) ../../../lib/

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm  -rf $(NAME)

re:	fclean all
