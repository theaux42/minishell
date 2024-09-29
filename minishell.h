/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:40:59 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 17:24:37 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include "libft/libft.h"

typedef struct s_prompt t_prompt;

typedef struct s_parser_state
{
    int in_double_str;
    int in_single_str;
    char *args;
    t_prompt *next;
} t_parser_state;

struct s_prompt
{
    char *value;
    int type;
    t_prompt *next;
};

typedef enum e_token_type
{
    COMMAND,                // Commande principale
    OPTION,                 // Option de commande (ex: -l)
    ARGUMENT,               // Argument de commande
    PIPE,                   // Pipe '|'
    REDIRECTION_INPUT,      // Redirection d'entrée '<'
    REDIRECTION_OUTPUT,     // Redirection de sortie '>'
    REDIRECTION_APPEND,     // Redirection avec ajout '>>'
    REDIRECTION_HEREDOC,    // Redirection avec heredoc '<<'
}   t_token_type;

// Structure pour un token
typedef struct s_token
{
    char *value;            // Valeur du token
    t_token_type type;      // Type du token
    struct s_token *next;   // Token suivant
} t_token;

// Structure pour une redirection
typedef struct s_redirection
{
    t_token_type type;              // Type de redirection
    char *file;                     // Nom du fichier de redirection
    struct s_redirection *next;     // Redirection suivante
} t_redirection;

// Structure pour une commande
typedef struct s_command
{
    char **argv;                    // Arguments de la commande
    t_redirection *redirections;    // Liste des redirections
    int pipes[2];                   // Descripteurs de pipe
    int prev_pipe;                  // Descripteur de lecture du pipe précédent
    int is_builtin;                 // Commande internes
    int is_absolute;                // Chemin absolu
    int is_last;                    // Dernière commande
    pid_t pid;                      // PID du processus
    struct s_command *next;         // Commande suivante (si pipe)
} t_command;

# define PROMPT "🤖 → "
# define CMD_NOT_FOUND 127

// testings
char *get_full_cmd(t_command *command);
// Fonctions de parsing
t_token_type get_redirection_type(char *str);
t_token_type define_type(char *str);
// Fonction d'exécution
void execute_command(t_command *command);
// Fonction de utils
void exit_error(char *msg);
void set_default_values(t_command *command);

# endif