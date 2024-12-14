/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:54:09 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 06:14:46 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *str)
{
	int	result;

	result = 0;
	if (ft_strncmp(str, "echo", 4) == 0)
		result = 1;
	else if (ft_strncmp(str, "cd", 2) == 0)
		result = 1;
	else if (ft_strncmp(str, "pwd", 3) == 0)
		result = 1;
	else if (ft_strncmp(str, "export", 6) == 0)
		result = 1;
	else if (ft_strncmp(str, "unset", 5) == 0)
		result = 1;
	else if (ft_strncmp(str, "env", 3) == 0)
		result = 1;
	else if (ft_strncmp(str, "exit", 4) == 0)
		result = 1;
	return (result);
}

int	exec_builtins(t_command *command, char ***env)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(command->tokens->value, "echo", 4) == 0)
		ret = ft_echo(command->tokens->next);
	else if (ft_strncmp(command->tokens->value, "cd", 2) == 0)
		ret = ft_cd(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "pwd", 3) == 0)
		ret = ft_pwd(*env);
	else if (ft_strncmp(command->tokens->value, "export", 6) == 0)
		ft_export(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "unset", 5) == 0)
		ft_unset(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "env", 3) == 0)
		ft_env(*env);
	return (ret);
}
