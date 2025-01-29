/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 05:31:06 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 21:08:17 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_env(char ***env)
{
	char *pwd;
	
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (1);
	*env = malloc(sizeof(char *));
	if (!*env)
		return (free(pwd), 1);
	(*env)[0] = NULL;
	set_env("PATH", "/bin:/usr/bin:/usr/local/bin", env);
	set_env("USER", "whoareu", env);
	set_env("PWD", pwd, env);
	set_env("OLDPWD", pwd, env);
	if (chdir(pwd) != 0)
		return (free(pwd), 1);
	free(pwd);
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
			exit_parent(ERR_CANT_INIT, minishell, true);
	}
	else
		dup_env(&minishell->env, env);
	if (!minishell->env)
		exit_parent(ERR_CANT_INIT, minishell, true);
	minishell->status = 0;
	minishell->commands = NULL;
	return (minishell);
}
