/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:31 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 18:18:10 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_cd_path(char *path, char **env)
{
	char	*new_path;

	if (path[0] == '/')
		return (path);
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
	return (NULL);
}

int	ft_cd(char *path, char ***env)
{
	char	cwd[1024];
	char	*oldpwd;
	char 	*new_path;

	oldpwd = get_env("PWD", *env);
	if (oldpwd)
		set_env("OLDPWD", oldpwd, env);
	new_path = get_cd_path(path, *env);
	if (!new_path)
		
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		set_env("PWD", cwd, env);
		v
		return (0);
	}
	perror("getcwd");
	return (1);
}
