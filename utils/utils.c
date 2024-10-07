/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/07 22:51:18 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_paths(char *command)
{
	char	*path;
	char	**paths;
	int		i;

	i = 0;
	path = getenv("PATH");
	paths = ft_split(path, ':');
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/");
		paths[i] = ft_strjoin(paths[i], command);
		i++;
	}
	return (paths);
}

char	*get_full_cmd(char *bin)
{
	char	**paths;
	char	*full_cmd;
	int		i;

	i = 0;
	if (bin[0] == '/')
	{
		if (access(bin, F_OK) == 0)
			return (bin);
		return (NULL);
	}
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
