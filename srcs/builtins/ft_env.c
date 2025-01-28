/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:56:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/27 15:54:37 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_with_empty(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
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
		show_with_empty(env);
	else
		show_without_empty(env);
	return (0);
}
