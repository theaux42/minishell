/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 23:49:47 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_paths(char *command)
{
	char	*path;
	char	*temp;
	char	**paths;
	int		i;

	i = 0;
	path = getenv("PATH");
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

char	*get_full_cmd(char *bin)
{
	char	**paths;
	char	*full_cmd;
	int		i;

	i = 0;
	if (bin[0] == '/' || bin[0] == '.')
		return (get_cmd(bin));
	paths = get_paths(bin);
	while (paths[i++])
	{
		if (access(paths[i], F_OK) == 0)
		{
			full_cmd = ft_strdup(paths[i]);
			ft_freesplit(paths);
			return (full_cmd);
		}
	}
	ft_freesplit(paths);
	return (NULL);
}

void	set_default_values(t_command *command)
{
	command->next = NULL;
	command->redirections = NULL;
	command->is_last = 0;
	command->is_builtin = 0;
	command->is_absolute = 0;
	command->pipes[0] = -1;
	command->pipes[1] = -1;
	command->prev_pipe = -1;
	command->pid = -1;
}

void	exit_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
