# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cquiana <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/07 22:58:59 by cquiana           #+#    #+#              #
#    Updated: 2021/03/07 22:59:49 by cquiana          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo_three

HEADER	=	philo_three.h

SRCS	=	parser.c \
			init_philo.c \
			utils.c \
			dinning.c \
			display.c \
			cleaning.c

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
