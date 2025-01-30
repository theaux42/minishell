/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:31 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 10:35:07 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	no_folder(char *path)
{
	if (!path || ft_strcmp(path, "~") == 0)
		ft_dprintf(2, ERR_CD_NO_HOME);
	else if (path[0] == '~')
		ft_dprintf(2, ERR_CD_NO_HOME);
	else
		ft_dprintf(2, ERR_CD_NO_FILE, path);
	free(path);
}

static char	*manage_relative_path(char *path, char **env)
{
	char	*new_path;
	char	*pwd;

	pwd = get_pwd(env);
	if (!pwd)
		return (NULL);
	new_path = ft_strjoin(pwd, path + 1);
	if (!new_path)
		return (free(pwd), NULL);
	free(pwd);
	return (new_path);
}

static char	* get_cd_path(char *path, char **env)
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
		return (manage_relative_path(path, env));
	if (path[0] == '~')
	{
		new_path = ft_strjoin(get_env("HOME", env), path + 1);
		if (!new_path)
			return (NULL);
		return (new_path);
	}
	return (ft_strdup(path));
}

int	ft_cd(t_token *token, char ***env)
{
	char	cwd[1024];
	char	*path;
	char	*oldpwd;
	char	*new_path;

	path = NULL;
	if (token)
		path = token->value;
	oldpwd = get_pwd(*env);
	if (oldpwd)
		set_env("OLDPWD", oldpwd, env);
	free(oldpwd);
	new_path = get_cd_path(path, *env);
	if (!new_path || access(new_path, F_OK) != 0)
		return (no_folder(new_path), 1);
	if (!ft_isfolder(new_path))
		return (free(new_path), ft_dprintf(2, ERR_IS_NOT_FOLDER, path), 1);
	if (chdir(new_path) != 0)
		return ((free(new_path), ft_dprintf(2, ERR_CD_NO_RIGHT, path)), 1);
	free(new_path);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (set_env("PWD", cwd, env), 0);
	ft_dprintf(2, ERR_CD_NO_RIGHT, path);
	return (1);
}
