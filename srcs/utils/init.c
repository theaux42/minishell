/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 05:31:06 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/14 02:43:34 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_default_values(t_command *command)
{
	command->next = NULL;
	command->redirections = NULL;
	command->is_last = 0;
	command->is_builtin = 0;
	command->is_absolute = 0;
	command->pipes[0] = -1;
	command->pipes[1] = -1;
	command->prev_pipe = -1;
	command->pid = -1;
}

char	**init_env(void)
{
	char	**env;

	env = malloc(sizeof(char *));
	if (!env)
		exit_error("malloc error");
	set_env("PATH", "/bin:/usr/bin:/usr/local/bin", &env);
	set_env("USER", "no_usr", &env);
	return (env);
}

t_minishell	*init_minishell(char **env)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		exit_error("malloc error");
	if (!env || !*env)
		minishell->env = init_env();
	else
		minishell->env = dup_env(env);
	minishell->status = 0;
	minishell->commands = NULL;
	return (minishell);
}
