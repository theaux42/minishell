# Makefile

NAME = execution
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -I/opt/homebrew/opt/readline/include
LDFLAGS = -L./libft -lft -L/opt/homebrew/opt/readline/lib -lreadline
SRCS = main.c exec/execution.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re