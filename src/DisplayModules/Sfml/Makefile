##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

NAME		=	arcade_sfml.so

CXXFLAGS	+=	-Wall -Werror -Wextra
CXXFLAGS	+=	-std=c++20 -fPIC
CXXFLAGS	+=	-I.. -I../../Core
CXXFLAGS	+=	-I/opt/sfml2/include # ArchLinux specific

ifeq ($(DEBUG),1)
CXXFLAGS += -g3
endif

LDFLAGS		+=	-shared
LDFLAGS		+=	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
LDFLAGS		+=	-L/opt/sfml2/lib # ArchLinux specific

CXX		=	g++

SRC		=	SfmlModule.cpp

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
