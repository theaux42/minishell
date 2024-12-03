/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/03 16:57:01 by ededemog         ###   ########.fr       */
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

// The getcwd() may be removed to be replaced by the path
// Contained in the env variable PATH

char	*get_cmd(char *command)
{
	char	*path;
	char	*relative_path;
	char	*temp;

	if (command[0] == '/')
	{
		if (access(command, F_OK) == 0)
			return (command);
	}
	else
	{
		path = getcwd(NULL, 0);
		temp = ft_strjoin(path, "/");
		relative_path = ft_strjoin(temp, command);
		free(temp);
		free(path);
		if (access(relative_path, F_OK) == 0)
			return (relative_path);
	}
	return (NULL);
}

char	*get_full_cmd(char *bin, char **env)
{
	char	**paths;
	char	*full_cmd;
	int		i;

	i = 0;
	if (bin[0] == '/' || bin[0] == '.')
		return (ft_strdup(get_cmd(bin)));
	paths = get_paths(bin, env);
	while (paths[i])
	{
		if (access(paths[i], F_OK) == 0)
		{
			full_cmd = ft_strdup(paths[i]);
			ft_freesplit(paths);
			return (full_cmd);
		}
		i++;
	}
	ft_freesplit(paths);
	return (NULL);
}

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

char	*ft_strjoin_multi(int count, char **strings)
{
	char	*res;
	size_t	len;
	size_t	i;

	len = 1;
	i = 0;
	if (count <= 0 || !strings)
		return (NULL);
	while (i < (size_t)count)
	{
		if (strings[i])
			len += ft_strlen(strings[i]);
		i++;
	}
	res = (char *)malloc(len * sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '\0';
	i = 0;
	while (i < (size_t)count)
	{
		if (strings[i])
			ft_strcat(res, strings[i]);
		i++;
	}
	return (res);
}