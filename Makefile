# Makefile

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3 -I/opt/homebrew/opt/readline/include -I.

LDFLAGS = -L./libft -lft -L/opt/homebrew/opt/readline/lib -lreadline

ENV_SRCS =  srcs/exec/env/del_env.c srcs/exec/env/dup_env.c \
       srcs/exec/env/get_env.c srcs/exec/env/set_env.c \

PARSER_SRCS = srcs/parsing/parser.c srcs/parsing/split.c

UTILS_SRCS = srcs/utils/free.c srcs/utils/split.c srcs/utils/parser.c \
	   srcs/utils/utils.c srcs/utils/init.c

EXEC_SRCS = srcs/exec/exec/exec.c srcs/exec/exec/exec_utils.c

BUILTINS_SRCS = srcs/builtins/cd.c srcs/builtins/pwd.c \
		srcs/builtins/builtins.c

REDIRECTION_SRCS = srcs/exec/redirection/redirection.c

SRCS = $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(EXEC_SRCS) $(BUILTINS_SRCS) $(REDIRECTION_SRCS)

MAIN = main.c
OBJS = $(SRCS:.c=.o) $(MAIN:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@cd libft && make
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@cd libft && make clean
	rm -f $(OBJS)

fclean: clean
	@cd libft && make fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
