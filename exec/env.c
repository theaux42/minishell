/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:29:35 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 21:30:17 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(char *key, char **env)
{
	char	**args;
	char	*result;
	int		i;

	if (!env || !key)
		return (NULL);
	i = 0;
	while (env[i])
	{
		args = ft_split(env[i], '=');
		if (!args)
			return (NULL);
		if (ft_strcmp(args[0], key) == 0)
		{
			result = ft_strdup(args[1]);
			if (!result)
				return (NULL);
			ft_freesplit(args);
			return (result);
		}
		ft_freesplit(args);
		i++;
	}
	return (NULL);
}

int	edit_env_value(char *key, char *value, char **env)
{
	char	**args;
	char	*new_env;
	int		i;

	i = 0;
	while (env[i])
	{
		args = ft_split(env[i], '=');
		if (!args)
			return (0);
		if (ft_strcmp(args[0], key) == 0)
		{
			new_env = ft_strjoin_double(key, "=", value);
			if (!new_env)
				return (ft_freesplit(args), 0);
			free(env[i]);
			env[i] = new_env;
			return (ft_freesplit(args), 1);
		}
		ft_freesplit(args);
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
		(ft_freesplit(result), NULL);
	result[i] = line;
	result[++i] = NULL;
	return (result);
}

void	set_env_value(char *key, char *value, char ***env)
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

char	**ft_dupenv(char **env)
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
