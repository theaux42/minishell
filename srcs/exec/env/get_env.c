/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:44:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/12 05:43:39 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env(char *key, char **env)
{
	char	**args;
	char	*result;
	int		i;

	if (!env || !key)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(key, env[i], ft_strlen(key) - 1) == 0)
		{
			args = ft_split(env[i], '=');
			if (!args)
				return (NULL);
			result = ft_strdup(args[1]);
			if (!result)
				return (NULL);
			ft_freesplit(args);
			return (result);
		}
		i++;
	}
	return (NULL);
}
