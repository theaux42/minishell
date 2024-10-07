# Makefile

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I/opt/homebrew/opt/readline/include
LDFLAGS = -L./libft -lft -L/opt/homebrew/opt/readline/lib -lreadline
SRCS = main.c exec/exec.c utils/utils.c parsing/parser.c parsing/utils.c parsing/split.c \
		exec/exec_utils.c parsing/split_utils.c utils/free.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@ cd libft && make
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@ cd libft && make clean
	rm -f $(OBJS)

fclean: clean
	@ cd libft && make fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re