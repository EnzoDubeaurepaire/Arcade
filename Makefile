##
## EPITECH PROJECT, 2025
## Arcade
## File description:
## Makefile
##

export DEBUG ?= 0

NAME		=	arcade

CORE_SRC	=	src/main.cpp \
			src/Loader/Loader.cpp \
			src/Core/Core.cpp \
			src/Core/MainMenu/MainMenu.cpp \
			src/Core/Objects/Sprite.cpp \
			src/Core/Objects/Text.cpp

OBJ		=	$(CORE_SRC:.cpp=.o)

GAME_LIBS	=	src/Games/Snake

DISPLAY_LIBS	=	src/DisplayModules/Sfml \
			        src/DisplayModules/Ncurses \
			        src/DisplayModules/Sdl \

CXXFLAGS	+=	-Wall -Wextra -Werror
CXXFLAGS	+=	-fno-gnu-unique -std=c++20 -ldl -fPIC
CXXFLAGS	+=	-I./src/Loader -I./src/Games -I./src/DisplayModules -I./src/Core -I./src/Core/MainMenu -I./src/Core/Objects

ifeq ($(DEBUG),1)
CXXFLAGS += -g3
endif

CC		=	g++

all:	$(NAME)

$(NAME):	games graphicals core

games:
	$(foreach lib, $(GAME_LIBS), make -C $(lib) $(DEBUG_FLAG);)

graphicals:
	$(foreach lib, $(DISPLAY_LIBS), make -C $(lib) $(DEBUG_FLAG);)

core:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CXXFLAGS)

debug:
	$(MAKE) fclean
	$(MAKE) DEBUG=1

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
