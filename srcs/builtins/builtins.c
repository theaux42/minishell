/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:54:09 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/12 06:55:19 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *str)
{
	if (ft_strncmp(str, "echo", 4) == 0)
		return (1);
	if (ft_strncmp(str, "cd", 2) == 0)
		return (1);
	if (ft_strncmp(str, "pwd", 3) == 0)
		return (1);
	if (ft_strncmp(str, "export", 6) == 0)
		return (1);
	if (ft_strncmp(str, "unset", 5) == 0)
		return (1);
	if (ft_strncmp(str, "env", 3) == 0)
		return (1);
	if (ft_strncmp(str, "exit", 4) == 0)
		return (1);
	return (0);
}
