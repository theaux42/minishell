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

BUILTINS_SRCS = srcs/builtins/ft_cd.c srcs/builtins/ft_pwd.c \
		srcs/builtins/builtins.c srcs/builtins/ft_echo.c srcs/builtins/ft_env.c \
		srcs/builtins/ft_export.c srcs/builtins/ft_unset.c 
		
REDIRECTION_SRCS = srcs/exec/redirection/redirection.c

SRCS = $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(EXEC_SRCS) $(BUILTINS_SRCS) $(REDIRECTION_SRCS) srcs/debug/debug.c

MAIN = main.c
OBJS = $(SRCS:.c=.o) $(MAIN:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "🔨 Building libft..."
	@cd libft && make
	@echo "🚀 Linking objects..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "✅ Build complete!"

%.o: %.c
	@echo "🛠️ Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "🧹 Cleaning object files..."
	@cd libft && make clean
	@rm -f $(OBJS)
	@echo "🧼 Clean complete!"

fclean: clean
	@echo "🧹 Cleaning executable..."
	@cd libft && make fclean
	@rm -f $(NAME)
	@echo "🧼 Full clean complete!"

re: fclean all

.PHONY: all clean fclean re
