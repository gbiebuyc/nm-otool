# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gbiebuyc <gbiebuyc@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/30 13:06:05 by gbiebuyc          #+#    #+#              #
#    Updated: 2020/01/30 13:06:11 by gbiebuyc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = src/common.c \
	src/endianness.c
OBJ = $(SRC:.c=.o)
CFLAGS = -I ./libft -g # -Wall -Wextra -Werror
LDFLAGS = -L ./libft -lft
.PHONY: all clean fclean re

all: libft/libft.a ft_nm ft_otool

libft/libft.a:
	make -C ./libft

ft_nm: $(OBJ) src/nm.o
	$(CC) $^ -o $@ $(LDFLAGS)

ft_otool: $(OBJ) src/otool.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	#make -C ./libft clean
	rm -rf $(OBJ)

fclean:
	#make -C ./libft fclean
	rm -rf $(OBJ)
	rm -rf ft_nm
	rm -rf ft_otool

re: fclean all
