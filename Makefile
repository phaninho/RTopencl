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
SRC_DIR = srcs/
OBJ_DIR = obj/

SRC_FILE = kernel_init.c \
libft/ft_atoi.c \
libft/ft_isdigit.c \
libft/ft_isspace.c \
libft/ft_itoa.c \
libft/ft_putendl.c \
libft/ft_putstr.c \
libft/ft_strcmp.c \
libft/ft_strdup.c \
libft/ft_strlen.c \
libft/ft_strprefix.c \
libft/ft_strsub.c \
libft/ft_strsuffix.c \
parser/checker.c \
parser/checker2.c \
parser/get_values.c \
parser/globals.c \
parser/init.c \
parser/init2.c \
parser/parse_args.c \
parser/parser.c \
parser/parser_utils.c \
parser/parser_utils2.c \
raytracer.c \
raytracer/camera.c \
raytracer/camera2.c \
raytracer/mouse.c \
raytracer/rt_env.c \
raytracer/screenshot.c \
raytracer/screenshot2.c \
thread/thread.c \
utils/error.c \
utils/file.c \
utils/math.c \
window/button.c \
window/button_draw.c \
window/button_exec.c \
window/button_exec2.c \
window/draw.c \
window/draw2.c \
window/event.c \
window/interface.c \
window/utils.c \
window/utils2.c \
window/utils3.c \
window/window_0.c \
window/window_1.c

OBJ_FILE = $(SRC_FILE:.c=.o)
SRCS = $(addprefix $(SRC_DIR),$(SRC_FILE))
OBJS = $(addprefix $(OBJ_DIR),$(OBJ_FILE))

SRC_ALL_DIR = window utils parser raytracer thread libft
OBJ_ALL_DIR = $(addprefix $(OBJ_DIR),$(SRC_ALL_DIR))

IFLAGS = -I $(INC_DIR) -I ./libsdl/include


CC = gcc -g $(FLAGS)
LIBFLAGS = -L ./libsdl/lib -lSDL2 -lSDL2_ttf -framework OPENCL
RM = /bin/rm -f

all: build $(NAME)

build :
	@mkdir -p $(OBJ_ALL_DIR)

install:
	@mkdir -p libsdl && cd libsdl && ../SDL2-2.0.5/configure --prefix=`pwd` && make && make install
	@echo "Make SDL2 :\033[1;32m DONE !\033[m"
	@mkdir -p libsdl && cd libsdl && ../SDL2_ttf-2.0.14/configure --prefix=`pwd` --with-sdl-prefix=`pwd` && make check && make install
	@echo "Make SDL2_TTF :\033[1;32m DONE !\033[m"

uninstall:
	rm -rf libsdl

$(NAME) : $(OBJS)
	@echo "Make Objects :\033[1;32m DONE !\033[m"
	$(CC) -o $@ $^ $(LIBFLAGS)
	@echo "Make $(NAME) :\033[1;32m DONE ``!\033[m"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -MMD $(IFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS)
	@echo "Make clean :\033[1;31m DONE !\033[m"

fclean : clean
	$(RM) -rf $(OBJ_DIR)
	$(RM) $(NAME)
	@echo "Make fclean :\033[1;31m DONE !\033[m"

re: fclean all

.PHONY: all clean fclean re build install
