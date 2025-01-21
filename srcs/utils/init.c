/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 05:31:06 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/21 12:15:25 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_default_values(t_command *command)
{
	command->next = NULL;
	command->redirections = NULL;
	command->is_last = 0;
	command->is_absolute = 0;
	command->pipes[0] = -1;
	command->pipes[1] = -1;
	command->prev_pipe = -1;
	command->pid = -1;
}

int	init_env(char ***env)
{
	*env = malloc(sizeof(char *));
	if (!*env)
		return (1);
	printf("Initialising the env\n");
	(*env)[0] = NULL;
	set_env("PATH", "/bin:/usr/bin:/usr/local/bin", env);
	set_env("USER", "whoareu", env);
	set_env("PWD", "/home", env);
	set_env("OLDPWD", "/home", env);
	print_env(*env);
	if (chdir(get_env("PWD", *env)) != 0)
		perror("chdir error");
	return (0);
}

t_minishell	*init_minishell(char **env)
{
	t_minishell	*minishell;

	minishell = ft_calloc(1, sizeof(t_minishell));
	if (!minishell)
		return (ft_dprintf(2, ERR_MALLOC), NULL);
	if (!env || !*env)
	{
		if (init_env(&minishell->env))
			return (ft_dprintf(2, ERR_MALLOC), NULL);
	}
	else
		dup_env(&minishell->env, env);
	minishell->status = 0;
	minishell->commands = NULL;
	return (minishell);
}
