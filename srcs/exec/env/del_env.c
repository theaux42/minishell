/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:42:11 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/06 22:29:23 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_env(char *key, char ***env)
{
	char	**result;
	int		i;
	int		j;

	result = malloc(sizeof(char *) * (ft_split_len(*env)));
	if (!result)
		return ;
	i = 0;
	j = -1;
	while ((*env)[++j])
	{
		if (ft_strncmp((*env)[j], key, ft_strlen(key) - 1) != 0)
		{
			result[i] = ft_strdup((*env)[j]);
			if (!result[i])
			{
				ft_freesplit(result);
				return ;
			}
			i++;
		}
	}
	result[i] = NULL;
	ft_freesplit(*env);
	*env = result;
}
