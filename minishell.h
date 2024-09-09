/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 12:40:59 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/09 15:42:52 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
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

# define COMMAND 1
# define ARGUMENT 2
# define OPTIONS 3
# define REDIRECTION 4
# define PIPE 5
# define SEMICOLON 6

# define PROMPT "🤖 → "

# endif