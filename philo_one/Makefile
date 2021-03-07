# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cquiana <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/07 20:54:11 by cquiana           #+#    #+#              #
#    Updated: 2021/03/07 21:57:52 by cquiana          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo_one

HEADER	=	philo_one.h

SRCS	=	parser.c \
			init_philo.c \
			utils.c \
			dinning.c \
			display.c

OBJ	=	$(SRCS:.c=.o)

FLAGS	=	-Wall -Wextra -Werror -pthread

%.o:%.c
	gcc $(FLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ) $(HEADER) main.c
	@gcc $(FLAGS) main.c $(OBJ) -o $(NAME)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
