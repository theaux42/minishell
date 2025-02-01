/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:56:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 12:16:01 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_show(char **env)
{
	int		i;
	char	current;

	current = 'A';
	while ((current <= 'Z') || (current >= 'a' && current <= 'z'))
	{
		i = 0;
		while (env[i])
		{
			if (env[i][0] == current)
				printf("export %s\n", env[i]);
			i++;
		}
		if (current == 'Z')
			current = 'a';
		else
			current++;
	}
}

void	show_without_empty(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (env[i][ft_strlen(env[i]) - 1] != '=')
			printf("%s\n", env[i]);
		i++;
	}
}

int	ft_env(char **env, bool show_empty)
{
	if (!env)
		return (ft_dprintf(2, ERR_NO_ENV), 1);
	if (show_empty)
		export_show(env);
	else
		show_without_empty(env);
	return (0);
}
