/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 09:27:30 by tbabou           ###   ########.fr       */
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
	if (!paths)
		return (NULL);
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

void	exit_error_parent(char *msg, t_minishell *minishell)
{
	printf("%s", msg);
	free_commands(minishell->commands);
	free_history(minishell->history);
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	exit(EXIT_FAILURE);
}

void	exit_error_child(char *msg, t_minishell *minishell, char *cmd,
		char **argv)
{
	perror(msg);
	free_commands(minishell->commands);
	free_history(minishell->history);
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	free(cmd);
	free(argv);
	exit(EXIT_FAILURE);
}
