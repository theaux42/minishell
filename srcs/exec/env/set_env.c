/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:42:39 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/12 05:39:50 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	edit_env_value(char *key, char *value, char **env)
{
	char	**args;
	char	*new_env;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key) - 1) == 0)
		{
			args = ft_split(env[i], '=');
			if (!args)
				return (0);
			new_env = ft_strjoin_double(key, "=", value);
			if (!new_env)
				return (ft_freesplit(args), 0);
			free(env[i]);
			env[i] = new_env;
			return (ft_freesplit(args), 1);
		}
		i++;
	}
	return (0);
}

char	**add_env_value(char *key, char *value, char **env)
{
	int		env_len;
	char	**result;
	char	*line;
	int		i;

	env_len = ft_split_len(env);
	result = malloc(sizeof(char *) * (env_len + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (env[i])
	{
		result[i] = ft_strdup(env[i]);
		if (!result[i])
			return (ft_freesplit(result), NULL);
		i++;
	}
	line = ft_strjoin_double(key, "=", value);
	if (!line)
		return (ft_freesplit(result), NULL);
	result[i] = line;
	result[++i] = NULL;
	return (result);
}

void	set_env(char *key, char *value, char ***env)
{
	int		result;
	char	**new_env;

	result = edit_env_value(key, value, *env);
	if (!result)
	{
		new_env = add_env_value(key, value, *env);
		if (!new_env)
			return ;
		ft_freesplit(*env);
		*env = new_env;
	}
}
