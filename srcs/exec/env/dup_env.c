/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:41:32 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 12:02:39 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_env(char ***dest_env, char **src_env)
{
	int	env_len;
	int	i;

	env_len = ft_split_len(src_env);
	*dest_env = malloc(sizeof(char *) * (env_len + 1));
	if (!*dest_env)
		return ;
	i = 0;
	while (src_env[i])
	{
		(*dest_env)[i] = ft_strdup(src_env[i]);
		if (!(*dest_env)[i])
		{
			ft_freesplit(*dest_env);
			*dest_env = NULL;
			return ;
		}
		i++;
	}
	(*dest_env)[i] = NULL;
}
