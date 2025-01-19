/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:42:11 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/19 17:37:01 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_var(char *key, char ***env)
{
	int		i;
	char	*value;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, ft_strlen(key) - 1) == 0)
		{
			value = ft_strchr((*env)[i], '=');
			if (value && *value)
				return (true);
		}
		i++;
	}
	return (false);
}

static char	**remove_var(char *key, char ***env)
{
	char	**result;
	int		i;
	int		j;

	result = malloc(sizeof(char *) * (ft_split_len(*env)));
	if (!result)
		return (NULL);
	i = 0;
	j = -1;
	while ((*env)[++j])
	{
		if (ft_strncmp((*env)[j], key, ft_strlen(key) - 1) != 0)
		{
			result[i] = ft_strdup((*env)[j]);
			if (!result[i])
				return (ft_freesplit(result), NULL);
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}

void	del_env(char *key, char ***env)
{
	char	**result;

	if (!check_var(key, env))
		return ;
	result = remove_var(key, env);
	if (!result)
		return ;
	ft_freesplit(*env);
	*env = result;
}

// void	old_del_env(char *key, char ***env)
// {
// 	char	**result;
// 	int		i;
// 	int		j;

// 	result = malloc(sizeof(char *) * (ft_split_len(*env)));
// 	if (!result)
// 		return ;
// 	i = 0;
// 	j = -1;
// 	ft_freesplit(*env);
// 	*env = result;
// }
