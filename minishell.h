/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:40:59 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 04:18:16 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
	COMMAND, // Commande principale
	OPTION, // Option de commande (ex: -l)
	ARGUMENT, // Argument de commande
	PIPE, // Pipe '|'
	REDIR_INPUT, // Redirection d'entrée '<'
	REDIR_OUTPUT, // Redirection de sortie '>'
	REDIR_APPEND, // Redirection avec ajout '>>'
	REDIR_HEREDOC, // Redirection avec heredoc '<<'
}				t_token_type;

// Structure pour un token
typedef struct s_token
{
	char			*value; // Valeur du token
	t_token_type	type; // Type du token
	struct s_token	*next; // Token suivant
}				t_token;

// Structure pour une redirection
typedef struct s_redirection
{
	t_token_type			type; // Type de redirection
	char					*file; // Nom du fichier de redirection
	struct s_redirection	*next; // Redirection suivante
}				t_redirection;

// Structure pour une commande
typedef struct s_command
{
	t_token				*tokens; // Liste des tokens
	t_redirection		*redirections; // Liste des redirections
	int					pipes[2]; // Descripteurs de pipe
	int					prev_pipe; // Descripteur de lecture du pipe précédent
	int					is_builtin; // Commande internes
	int					is_absolute; // Chemin absolu
	int					is_last; // Dernière commande
	pid_t				pid; // PID du processus
	struct s_command	*next; // Commande suivante (si pipe)
}				t_command;

typedef struct s_minishell
{
	t_command	*commands;
	char		**env;
	int			status;
}				t_minishell;

# define PROMPT "ᓚᘏᗢ"
# define CMD_NOT_FOUND 127

// testings
char			*get_full_cmd(char *bin, char **env);
char			*ft_token_value(char *value);
// === PARSING ===
// Functions of parsing/expand.c
bool			expand_commands(t_command *commands, t_minishell *minishell);
void			expand_tokens(t_token *tokens, t_minishell *minishell);

// Functions of parsing/quotes.c
int				check_missused_quotes(const char *str);
size_t			ft_strlen_quote(const char *str, char quote);
char			*process_quote(char *line);

// Functions of parsing/expand_utils.c
bool			need_expansion(char *value);
char			*get_env_var(char *key, t_minishell *minishell);
char			*get_current_key(char *line, int *i);
bool			is_valid_key(char *key);

// Functions of parsing/parser.c
t_command		*get_commands(char *line, t_minishell *minishell);

// Functions of parsing/utils.c
t_token_type	get_redirection_type(char *str);
t_token_type	get_tokens_type(char *str, int pos);

// Functions of parsing/split_utils.c
int				ft_ms_isspace(char c);
int				quote_manager(char current, int is_in_arg);

// Functions of parsing/redirections.c
void			parse_redirections(t_command *command);
// Functions of parsing/split.c
char			**ft_ms_split(char *line);
int				ft_prompt_length(char *line);

// === EXECUTION ===
// Fonction d'exécution
void			execute_commands(t_minishell *minishell);
int				execution(char *cmd, t_command *command, char ***env);

// Functions of exec/exec/exec_2.c
bool			needs_parent_execution(char *cmd_name);
int				execute_external_command(t_minishell *minishell,
					t_command *command, t_token *tokens);
int				exec_cmd(t_minishell *minishell, t_command *command);

// Fonction Utils
void			init_pipes(t_command *commands);
void			wait_for_children(t_minishell *minishell);
int				count_arguments(t_command *command);
int				fill_arguments(char **argv, t_command *command);
void			no_cmd_handler(t_command *current);
// Functions of utils/prompt.c
char			*nice_prompt(char **env);

// Functions of utils/prompt_utils.c
char			*replace_home_with_tilde(char *pwd, char *home);
char			*append_path_segment(char *current_path, char *segment,
					bool add_slash);

// Functions of exec/redirection/redirection.c
void			apply_redirections(t_redirection *redirections);

// Fonction env
char			*get_env(char *key, char **env);
void			del_env(char *key, char ***env);
void			dup_env(char ***dest_env, char **src_env);
void			set_env(char *key, char *value, char ***env);

// === BUILTINS ===
int				is_builtin(char *str);
int				exec_builtins(t_command *command, char ***env);

// Les builtins
int				ft_echo(t_token *tokens);
int				ft_pwd(char **env);
int				ft_cd(t_token *token, char ***env);
int				ft_env(char **env);
int				ft_export(t_token *tokens, char ***env);
int				ft_unset(t_token *tokens, char ***env);

// === UTILS ===
// Fonction de utils
void			exit_error(char *msg);
void			set_default_values(t_command *command);
// Fonction de free
void			free_commands(t_command *commands);
// Fonction d'initialisation
t_minishell		*init_minishell(char **env);

// === DEBUG ===
// Fonction de debug
void			print_commands(t_command *commands);
void			print_tokens(t_token *tokens);
void			print_tokens2(t_token *tokens, char separator);
void			print_env(char **env);

char			*type_str(t_token_type type);

// Fonction de testing
#endif