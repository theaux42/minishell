/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:41:32 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 23:49:42 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**dup_env(char **env)
{
	int		env_len;
	char	**result;
	int		i;

	env_len = ft_split_len(env);
	result = malloc(sizeof(char *) * (env_len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env[i])
	{
		result[i] = ft_strdup(env[i]);
		if (!result[i])
			return (NULL);
		i++;
	}
	result[i] = NULL;
	return (result);
}
