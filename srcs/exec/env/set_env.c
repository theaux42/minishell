/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 23:42:39 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 21:31:40 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	edit_env_value(char *key, char *value, char **env)
{
	char	*new_env;
	int		key_len;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
		{
			new_env = ft_strjoin_double(key, "=", value, 0);
			if (!new_env)
				return (0);
			free(env[i]);
			env[i] = new_env;
			return (1);
		}
		i++;
	}
	return (0);
}

char	**add_env_value(char *key, char *value, char **env)
{
	int		env_len;
	char	**new_env;
	int		i;

	i = 0;
	env_len = ft_split_len(env);
	new_env = malloc(sizeof(char *) * (env_len + 2));
	if (!new_env)
		return (NULL);
	ft_memset(new_env, 0, sizeof(char *) * (env_len + 2));
	while (env[i] && i < env_len)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return (ft_freesplit(new_env), NULL);
		i++;
	}
	new_env[env_len] = ft_strjoin_double(key, "=", value, 0);
	if (!new_env[env_len])
		return (ft_freesplit(new_env), NULL);
	new_env[env_len + 1] = NULL;
	return (new_env);
}

void	set_env(char *key, char *value, char ***env)
{
	char	**new_env;

	if (!edit_env_value(key, value, *env))
	{
		new_env = add_env_value(key, value, *env);
		if (!new_env)
			return ;
		ft_freesplit(*env);
		*env = new_env;
	}
}
