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

SRC = main2.c \
	get_type_char.c \
	symbols.c \
	parse_header.c \
	parse_sections.c \
	parse_commands.c \
	parse_fat.c \
	print_text_section.c \
	print_filename.c \
	endianness.c
OBJ = $(addprefix obj/, $(SRC:.c=.o))
CFLAGS = -I ./libft -Wall -Wextra -Werror
LDFLAGS = -L ./libft -lft
.PHONY: all clean fclean re

all: libft/libft.a obj/ ft_nm ft_otool

libft/libft.a:
	make -C ./libft

obj/:
	mkdir -p obj/

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

ft_nm: $(OBJ) obj/nm.o
	$(CC) $(LDFLAGS) -o $@ $^

ft_otool: $(OBJ) obj/otool.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	make -C ./libft clean
	rm -rf obj/

fclean:
	make -C ./libft fclean
	rm -rf obj/
	rm -rf ft_nm
	rm -rf ft_otool

re: fclean all
