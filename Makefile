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

SRC = src/main2.c \
	src/get_type_char.c \
	src/symbols.c \
	src/parse_header.c \
	src/parse_sections.c \
	src/parse_commands.c \
	src/parse_fat.c \
	src/print_text_section.c \
	src/print_filename.c \
	src/endianness.c
OBJ = $(SRC:.c=.o)
CFLAGS = -I ./libft -Wall -Wextra -Werror
LDFLAGS = -L ./libft -lft
.PHONY: all clean fclean re

all: libft/libft.a ft_nm ft_otool

libft/libft.a:
	make -C ./libft

ft_nm: $(OBJ) src/nm.o
	$(CC) $(LDFLAGS) -o $@ $^

ft_otool: $(OBJ) src/otool.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	make -C ./libft clean
	rm -rf $(OBJ)

fclean:
	make -C ./libft fclean
	rm -rf $(OBJ) src/nm.o src/otool.o
	rm -rf ft_nm
	rm -rf ft_otool

re: fclean all
