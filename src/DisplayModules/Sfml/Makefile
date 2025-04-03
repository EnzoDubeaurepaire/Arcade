##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME	=		arcade_sfml.so

CXXFLAGS	+=	-Wall -Werror -Wextra -g
CXXFLAGS	+=	-std=c++20 -fPIC
CXXFLAGS	+=	-I ../ -I ../../Core

LDFLAGS	+=	-L/opt/sfml2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
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
	rm  -rf ../../../lib/$(NAME)

re:	fclean all
