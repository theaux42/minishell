/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:56:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/24 23:55:40 by tbabou           ###   ########.fr       */
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
	while (env[i])
		i++;
	return (0);
}
