/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/20 14:40:40 by tbabou           ###   ########.fr       */
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
	(free(temp), free(path));
	if (access(relative_path, F_OK) == 0 && access(relative_path, X_OK) == 0)
		return (relative_path);
	free(relative_path);
	return (NULL);
}

static char	*get_cmd(char *command)
{
	char	*path;
	char	*relative_path;

	if (command[0] == '/')
		return (ft_strdup(command));
	else
	{
		path = getcwd(NULL, 0);
		relative_path = get_relative_cmd(command, path);
		if (relative_path)
			return (ft_strdup(relative_path));
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
	ft_dprintf(2, "%s", msg);
	if (minishell->commands)
		free_commands(minishell->commands);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	exit(EXIT_FAILURE);
}

void	exit_error_child(char *msg, t_minishell *minishell, char *cmd,
		char **argv)
{
	if (msg != NULL)
		ft_dprintf(2, "%s", msg);
	free_commands(minishell->commands);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	free(cmd);
	free(argv);
	exit(EXIT_FAILURE);
}
