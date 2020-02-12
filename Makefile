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

NAME = ft_nm
SRC = src/nm.c
OBJ = $(SRC:.c=.o)
CFLAGS = -I ./libft -g # -Wall -Wextra -Werror
LDFLAGS = -L ./libft -lft
.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	make -C ./libft
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	#make -C ./libft clean
	rm -rf $(OBJ)

fclean:
	#make -C ./libft fclean
	rm -rf $(OBJ)
	rm -rf $(NAME)

re: fclean all
