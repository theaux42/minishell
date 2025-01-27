/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/26 19:04:48 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char *command, char **env)
{
	char	*path;
	char	*temp;
	char	**paths;
	int		i;

	i = 0;
	path = get_env("PATH", env);
	if (!path || !*path)
		return (NULL);
	paths = ft_split(path, ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		free(paths[i]);
		paths[i] = ft_strjoin(temp, command);
		free(temp);
		i++;
	}
	return (paths);
}

static char	*get_relative_cmd(char *command, char *path)
{
	char	*relative_path;
	char	*temp;

	if (!path)
		return (NULL);
	temp = ft_strjoin(path, "/");
	relative_path = ft_strjoin(temp, command);
	(free(path), free(temp));
	if (access(relative_path, F_OK) == 0)
		return (relative_path);
	free(relative_path);
	return (NULL);
}

static char	*get_cmd(char *command)
{
	char	*path;
	char	*relative_path;

	if (command[0] == '/')
	{
		if (access(command, F_OK | X_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	else
	{
		path = getcwd(NULL, 0);
		relative_path = get_relative_cmd(command, path);
		if (relative_path)
		{
			if (access(relative_path, X_OK) == 0)
				return (relative_path);
			else
			{
				free(relative_path);
				return (ft_strdup(command));
			}
		}
	}
	return (NULL);
}

char	*get_full_cmd(char *bin, char **env)
{
	char	**paths;
	char	*full_cmd;
	int		i;

	i = 0;
	if (ft_strcmp(bin, ".") == 0 || ft_strcmp(bin, "..") == 0)
		return (NULL);
	if (is_builtin(bin))
		return (ft_strdup(bin));
	if (bin[0] == '/' || ft_strncmp(bin, "./", 2) == 0)
		return (get_cmd(bin));
	paths = get_paths(bin, env);
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		if (access(paths[i], F_OK) == 0)
		{
			full_cmd = ft_strdup(paths[i]);
			return (ft_freesplit(paths), full_cmd);
		}
		i++;
	}
	return (ft_freesplit(paths), NULL);
}

int	ft_cmdlen(t_token *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}
