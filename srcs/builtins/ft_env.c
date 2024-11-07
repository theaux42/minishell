/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:56:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 17:31:40 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
	{
		fprintf(stderr, "env: No environment variables found\n");
		return (1);
	}
	printf("[DEBUG] ft_env: Listing environment variables\n");
	while (env[i])
	{
		printf("[DEBUG] env[%d]: %s\n", i, env[i]);
		i++;
	}
	return (0);
}
