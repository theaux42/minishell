/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:40:59 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 23:53:33 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef DEBUG_MODE
#  define DEBUG_MODE 0
# endif

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	COMMAND,
	OPTION,
	ARGUMENT,
	PIPE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_TARGET,
}								t_token_type;

// Structure pour un token
typedef struct s_token
{
	char						*value;
	t_token_type				type;
	struct s_token				*next;
}								t_token;

// Structure pour une redirection
typedef struct s_redirection
{
	t_token_type				type;
	char						*file;
	struct s_redirection		*next;
}								t_redirection;

// Structure pour une commande
typedef struct s_command
{
	t_token						*tokens;
	t_redirection				*redirections;
	int							pipes[2];
	int							prev_pipe;
	int							is_absolute;
	int							is_last;
	pid_t						pid;
	struct s_command			*next;
}								t_command;

typedef struct s_history
{
	char						*command;
	struct s_history			*next;
}								t_history;

typedef struct s_minishell
{
	t_command					*commands;
	t_history					*history;
	char						**env;
	char						*line;
	int							status;
	int							cmd_count;
	int							fds[2];
}								t_minishell;

# define DEBUG_MSG "Debug mode enabled\nCommands will be printed.\n"
# define DEBUG_STATUS_MSG "Exited with status: %i\n"
# define DEBUG_EXEC_PARENT "Executing builtins inside the parent process\n"
# define DEBUG_EXEC_CHILD "Executing builtins inside the child process\n"

# define DEFAULT_PROMPT "ᓚᘏᗢ $ "
# define HEREDOC_PROMPT "heredoc> "

# define CMD_NOT_FOUND -10
# define CMD_NO_RIGHT -20
# define CMD_IS_FOLDER -25
# define CMD_PARENT_BUILTINS -30

# define MSG_COREDUMP "Quit (core dumped)\n"

# define ERR_CANT_INIT "minishell: unable to initialize for some reasons.\n"
# define ERR_CMD_NOT_FOUND "minishell: %s: command not found\n"
# define ERR_EMPTY_CMD "minishell: parse error near `|'\n"
# define ERR_UNCLOSED_QUOTES "minishell: unclosed quotes\n"
# define ERR_DEBUG "minishell: debug message\n"
# define ERR_MALLOC "minishell: a malloc failed\n"
# define ERR_BAD_REDIRECTION "minishell: badly formated redirections.\n"
# define ERR_PIPE_FAIL "minishell: pipe failed\n"
# define ERR_TOO_MANY_ARGS "minishell: %s: too many arguments\n"
# define ERR_NUM_ARG "minishell: %s: numeric argument required\n"
# define ERR_EXIT_NUM "minishell: exit: %s: numeric argument required\n"
# define ERR_EXPORT_INVALID "minishell: export: %s: invalid syntax\n"
# define ERR_NO_FILE "minishell: %s: No such file or directory\n"
# define ERR_EXPORT_INVALID_ID "minishell: export: %s: not a valid identifier\n"
# define ERR_UNSET_INVALID_ID "minishell: unset: %s: not a valid identifier\n"
# define ERR_NOT_A_TTY "minishell: this is not a tty!\n"
# define ERR_NO_RIGHT "minishell: %s: Permission denied\n"
# define ERR_CD_NO_RIGHT "minishell: cd: %s: Permission denied\n"
# define ERR_DUP_FD "minishell: dup2 failed\n"
# define ERR_CD_NO_FILE "minishell: cd: %s: No such file or directory\n"
# define ERR_CD_NO_HOME "minishell: cd: HOME not set\n"
# define ERR_NO_PWD "minishell: Unable to get PWD after two tries.\n"
# define ERR_NO_ENV "minishell: env: cannot find environment variable\n"
# define ERR_IS_FOLDER "minishell: %s: Is a directory\n"
# define ERR_IS_NOT_FOLDER "minishell: %s: Is not a directory\n"
# define ERR_EXECVE "minishell: execve failed\n"

extern volatile sig_atomic_t	g_signal;

// testings
char							*get_full_cmd(char *bin, char **env);
// === PARSING ===
// Functions of parsing/expand.c
bool							expand_commands(t_command *commands,
									t_minishell *minishell);
char							*expand_line(char *line,
									t_minishell *minishell);

// Functions of parsing/quotes.c
int								check_missused_quotes(const char *str);
char							*process_quote(char *line);

// Functions of parsing/expand_utils.c
bool							need_expansion(char *value);
char							*get_env_var(char *key, t_minishell *minishell);
char							*get_current_key(char *line, int *i);
bool							is_valid_key(char *key);
bool							check_commands(t_command *commands);

// Functions of parsing/parser.c
t_command						*get_commands(char *line,
									t_minishell *minishell);

// Functions of parsing/utils.c
t_token_type					get_redirection_type(char *str);
t_token_type					get_tokens_type(char *str, int pos);

// Functions of parsing/check.c
bool							check_empty_pipes(char **tokens);
int								get_active_quotes(char *line, int n);

// Functions of parsing/split_utils.c
int								ft_ms_isspace(char c);
int								quote_manager(char current, int is_in_arg);
int								handle_token(char *line, int *i,
									int *is_in_arg);
int								process_redir(char **split, char *line, int *i,
									int *k);
// Functions of parsing/redirections.c
bool							parse_redirections(t_command *command,
									t_minishell *minishell);

// Functions of parsing/split.c
int								copy_arg(char **split, char *line, int j,
									int k);
char							**ft_ms_split(char *line);
int								ft_prompt_length(char *line);

// === EXECUTION ===
// Fonction d'exécution
void							execute_commands(t_minishell *minishell);
int								execution(char *cmd, t_command *command,
									t_minishell *minishell);
void							execute_child(char *cmd, t_command *command,
									t_minishell *minishell, char **argv);

// Functions of exec/exec/exec_2.c
int								exec_cmd(t_minishell *minishell,
									t_command *command);
int								ft_cmd_count(t_command *commands);

// Fonction Utils
void							init_pipes(t_command *commands,
									t_minishell *minishell);
void							wait_for_children(t_minishell *minishell);
int								count_arguments(t_command *command);
int								fill_arguments(char **argv, t_command *command);
int								ft_cmdlen(t_token *lst);
// Functions of utils/prompt.c
char							*nice_prompt(char **env);

// Functions of utils/prompt_utils.c
char							*replace_home_with_tilde(char *pwd, char *home);
char							*append_path_segment(char *current_path,
									char *segment, bool add_slash);

// Functions of exec/redirection/redirection.c
int								exec_redirections(t_redirection *redirections,
									t_minishell *minishell);
int								parent_redir(t_command *command,
									t_minishell *minishell);

// Fonction env
char							*get_env(char *key, char **env);
void							del_env(char *key, char ***env);
void							dup_env(char ***dest_env, char **src_env);
void							set_env(char *key, char *value, char ***env);

// === BUILTINS ===
int								is_builtin(char *str);
int								parent_builtins(t_command *command,
									t_minishell *minishell);
int								child_builtins(char **argv, char *cmd,
									t_command *command, t_minishell *minishell);

// Les builtins
int								ft_echo(t_token *tokens);
int								ft_pwd(char **env);
char							*get_pwd(char **env);
int								ft_cd(t_token *token, char ***env);
int								ft_env(char **env, bool show_empty);
int								ft_export(t_token *tokens, char ***env);
int								ft_unset(t_token *tokens, char ***env);
int								ft_exit(t_token *token, t_minishell *minishell,
									bool msg);

// === SIGNALS ===
// Functions of signals/signals.c
void							set_signal_child(void);
int								sig_event(void);
void							signal_handler(int sig);
void							ft_signal(void);

// === UTILS ===
// Fonction de utils
void							exit_parent(char *msg, t_minishell *minishell,
									bool is_error);
void							exit_child(char *msg, t_minishell *minishell,
									char *cmd, char **argv);
bool							ft_isfolder(char *path);

// Fonction de parsing
t_command						*init_new_command(void);
t_token							*create_arg_token(char *value, t_token *next);
int								split_expanded_token(t_token *token,
									char **split);

// Fonction d'expansion
char							*expand_line(char *line,
									t_minishell *minishell);
char							*expand_env_var(char *line, int *i,
									char *new_line, t_minishell *minishell);

// Fonction de redirection (parsing)
bool							handle_heredoc_parsing(t_token *current,
									char **value, t_minishell *minishell);
bool							handle_simple_redir(t_token *current,
									char **value, t_minishell *minishell);

// fonctions de redirection (exec)
void							close_fds(int *prev_fd, t_command *current);
int								handle_no_cmd_redir(t_command *command,
									t_minishell *minishell);
bool							handle_heredoc_exec(char *content,
									t_minishell *minishell);

// Fonction de free
void							free_commands(t_command *commands);
void							free_token(t_token *token);
void							free_tokens(t_token *tokens);
void							ft_free_builtins(t_minishell *minishell);
void							free_redirections(t_redirection *redirections);
// Fonction d'initialisation
t_minishell						*init_minishell(char **env);

// Fonction d'erreur
bool							validate_commands(t_command *commands,
									t_minishell *minishell);

// === DEBUG ===
// Fonction de debug
void							print_commands(t_command *commands);
void							print_tokens(t_token *tokens);
char							*type_str(t_token_type type);

// Fonction de testing
#endif