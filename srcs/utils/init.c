/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 05:31:06 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 17:32:00 by tbabou           ###   ########.fr       */
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

void	init_env(char ***env)
{
	*env = malloc(sizeof(char *));
	if (!*env)
		exit_error("malloc error");
	(*env)[0] = NULL;
	set_env("PATH", "/bin:/usr/bin:/usr/local/bin", env);
	set_env("USER", "whoareu", env);
	set_env("PWD", "/home", env);
	set_env("OLDPWD", "/home", env);
	print_env(*env);
	if (chdir(get_env("PWD", *env)) != 0)
		perror("chdir error");
}

t_minishell	*init_minishell(char **env)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		exit_error("malloc error");
	if (!env || !*env)
		init_env(&minishell->env);
	else
		dup_env(&minishell->env, env);
	minishell->status = 0;
	minishell->commands = NULL;
	return (minishell);
}
