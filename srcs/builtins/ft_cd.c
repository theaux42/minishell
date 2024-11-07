/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:31 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 20:06:49 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cd_path(char *path, char **env)
{
	char	*new_path;

	if (path == NULL || ft_strcmp(path, "~") == 0)
	{
		if (!get_env("HOME", env))
			return (NULL);
		return (ft_strdup(get_env("HOME", env)));
	}
	if (path[0] == '/')
		return (ft_strdup(path));
	if (path[0] == '.' && path[1] == '/')
	{
		new_path = ft_strjoin(get_env("PWD", env), path + 1);
		if (!new_path)
			exit_error("malloc error");
		return (new_path);
	}
	if (path[0] == '~')
	{
		new_path = ft_strjoin(get_env("HOME", env), path + 1);
		if (!new_path)
			exit_error("malloc error");
		return (new_path);
	}
	return (ft_strdup(path));
}

int	ft_cd(char *path, char ***env)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*new_path;

	oldpwd = get_env("PWD", *env);
	if (oldpwd)
		set_env("OLDPWD", oldpwd, env);
	new_path = get_cd_path(path, *env);
	if (!new_path)
		return (perror("cd"), 1);
	if (chdir(new_path) != 0)
	{
		free(new_path);
		return (perror("cd"), 1);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		free(new_path);
		set_env("PWD", cwd, env);
		return (0);
	}
	free(new_path);
	perror("getcwd");
	return (1);
}
