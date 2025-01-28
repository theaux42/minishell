/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 10:14:03 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(char **env)
{
	char	*pwd;

	pwd = get_env("PWD", env);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			ft_dprintf(2, ERR_NO_PWD);
	}
	else
		pwd = ft_strdup(pwd);
	if (!pwd)
		return (NULL);
	return (pwd);
}

int	ft_pwd(char **env)
{
	char	*pwd;

	(void)env;
	pwd = get_pwd(env);
	if (!pwd)
		return (1);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
