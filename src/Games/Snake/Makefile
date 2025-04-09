##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME		=	arcade_snake.so

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-std=c++20 -fPIC
CXXFLAGS	+=	-I.. -I../../Core -I../../Core/Objects

LDFLAGS	+=	-shared
ifeq ($(DEBUG),1)
CXXFLAGS += -g3
endif

LDFLAGS		+=	-shared

CXX		=	g++

SRC		=	Snake.cpp \
			../../Core/Objects/Sprite.cpp \
			../../Core/Objects/Text.cpp

OBJ		=	$(SRC:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)
	mv $(NAME) ../../../lib/

clean:
	rm -rf $(OBJ)

fclean:	clean
	rm -rf ../../../lib/$(NAME)

re:	fclean all

