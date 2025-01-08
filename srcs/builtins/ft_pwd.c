/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 07:52:51 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(char **env)
{
	char	*pwd;

	if (!env)
		return (1);
	pwd = get_env("PWD", env);
	if (!pwd)
	{
		pwd = getcwd(NULL, 0);
		if (!pwd)
			return (1);
		printf("%s\n", pwd);
		free(pwd);
	}
	else
	{
		printf("%s\n", pwd);
	}
	return (0);
}
