# Makefile

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3 -I/opt/homebrew/opt/readline/include -I.

LDFLAGS = -L./libft -lft -L/opt/homebrew/opt/readline/lib -lreadline

ENV_SRCS =  srcs/exec/env/del_env.c srcs/exec/env/dup_env.c \
       srcs/exec/env/get_env.c srcs/exec/env/set_env.c \

PARSER_SRCS = srcs/parsing/parser.c srcs/parsing/split.c \
	   srcs/parsing/expand.c srcs/parsing/expand_utils.c srcs/parsing/quotes.c \
	   srcs/parsing/redirections.c

UTILS_SRCS = srcs/utils/free.c srcs/utils/split_utils.c srcs/utils/parser.c \
	   srcs/utils/utils.c srcs/utils/init.c srcs/utils/prompt.c \
	   srcs/utils/prompt_utils.c srcs/utils/error.c 

EXEC_SRCS = srcs/exec/exec/exec.c srcs/exec/exec/exec_utils.c \
	    srcs/exec/exec/exec_2.c

SIGNALS_SRCS = srcs/signals/signals.c \

HISTORY_SRCS = srcs/history/history.c \

BUILTINS_SRCS = srcs/builtins/ft_cd.c srcs/builtins/ft_pwd.c \
		srcs/builtins/builtins.c srcs/builtins/ft_echo.c srcs/builtins/ft_env.c \
		srcs/builtins/ft_export.c srcs/builtins/ft_unset.c \
		srcs/builtins/ft_history.c srcs/builtins/ft_exit.c
		
REDIRECTION_SRCS = srcs/exec/redirection/redirection.c

SRCS = $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(EXEC_SRCS) $(BUILTINS_SRCS) \
	$(REDIRECTION_SRCS) $(SIGNALS_SRCS) $(HISTORY_SRCS) srcs/debug/debug.c

MAIN = main.c
OBJS = $(SRCS:.c=.o) $(MAIN:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "\n🔨 Building libft...\n"
	@cd libft && make --no-print-directory
	@echo "🚀 Linking objects..."
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "✅ Build complete!"

%.o: %.c
	@echo "🛠️ Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "🧹 Cleaning object files..."
	@cd libft && make clean --no-print-directory
	@rm -f $(OBJS)
	@echo "🧼 Clean complete!"

fclean: clean
	@echo "🧹 Cleaning executable..."
	@cd libft && make fclean --no-print-directory
	@rm -f $(NAME)
	@echo "🧼 Full clean complete!\n"

re: fclean all

.PHONY: all clean fclean re
