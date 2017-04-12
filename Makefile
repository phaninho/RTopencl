# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeexpert <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/12 13:47:45 by jeexpert          #+#    #+#              #
#    Updated: 2017/04/12 13:47:47 by jeexpert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = RT
INC_DIR = includes
SRC_DIR = srcs
OBJ_DIR = obj

SRCS = $(shell find $(SRC_DIR) -name '*.c' -type f)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Arborescence de fichier
SRC_ALL_DIR = $(shell find $(SRC_DIR) -type d)
OBJ_ALL_DIR = $(SRC_ALL_DIR:$(SRC_DIR)%=$(OBJ_DIR)%)

IFLAGS = -I $(INC_DIR) -I ./libsdl/include -I ./libpng/include
#FLAGS = -Wall -Werror -Wextra -O2 -O3

CC = gcc -g $(FLAGS)
LIBFLAGS = -L ./libsdl/lib -lSDL2 -lSDL2_ttf -L ./libpng/lib -lpng -framework OPENCL
RM = /bin/rm -f

all: build $(NAME)

build :
	@mkdir -p $(OBJ_ALL_DIR)

install:
	@mkdir -p libsdl && cd libsdl && ../SDL2-2.0.5/configure --prefix=`pwd` && make && make install
	@echo "Make SDL2 :\033[1;32m DONE !\033[m"
	@mkdir -p libpng && cd libpng && ../libpng-1.6.26/configure --prefix=`pwd` && make check && make install
	@echo "Make LIBPNG :\033[1;32m DONE !\033[m"
	@mkdir -p libsdl && cd libsdl && ../SDL2_ttf-2.0.14/configure --prefix=`pwd` --with-sdl-prefix=`pwd` && make check && make install
	@echo "Make SDL2_TTF :\033[1;32m DONE !\033[m"

uninstall:
	rm -rf libpng
	rm -rf libsdl

$(NAME) : $(OBJS)
	@echo "Make Objects :\033[1;32m DONE !\033[m"
	$(CC) -o $@ $^ $(LIBFLAGS)
	@echo "Make $(NAME) :\033[1;32m DONE !\033[m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -MMD $(IFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS)
	@echo "Make clean :\033[1;31m DONE !\033[m"

fclean : clean
	$(RM) -rf $(OBJ_ALL_DIR)
	$(RM) $(NAME)
	@echo "Make fclean :\033[1;31m DONE !\033[m"

re: fclean all

.PHONY: all clean fclean re build install
