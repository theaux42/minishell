/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:56:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 07:48:34 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
	{
		printf("env: No environment variables found\n");
		return (1);
	}
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
