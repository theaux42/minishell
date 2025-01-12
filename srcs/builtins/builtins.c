/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:54:09 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/12 09:55:25 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cmdlen(t_token *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

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

bool	is_valid_args(t_token *tokens, char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0)
		if (ft_cmdlen(tokens) > 2)
			return (printf(ERR_TOO_MANY_ARGS, "cd"), false);
	return (true);
}

int	builtins(t_command *command, char ***env, t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (ft_strncmp(command->tokens->value, "echo", 4) == 0)
		ret = ft_echo(command->tokens->next);
	else if (ft_strncmp(command->tokens->value, "cd", 2) == 0)
		ret = ft_cd(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "pwd", 3) == 0)
		ret = ft_pwd(*env);
	else if (ft_strncmp(command->tokens->value, "exit", 4) == 0)
		ret = ft_exit(command->tokens->next, minishell);
	else if (ft_strncmp(command->tokens->value, "export", 6) == 0)
		ret = ft_export(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "unset", 5) == 0)
		ret = ft_unset(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "env", 3) == 0)
		ret = ft_env(*env);
	return (ret);
}

int	exec_builtins(t_command *command, t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (is_valid_args(command->tokens, command->tokens->value))
		ret = builtins(command, &minishell->env, minishell);
	else
		ret = 1;
	if (!needs_parent_execution(command->tokens->value))
		ft_free_builtins(minishell);
	return (ret);
}

int	exec_builtins_2(char **argv, char *cmd, t_command *command,
		t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (is_valid_args(command->tokens, command->tokens->value))
		ret = builtins(command, &minishell->env, minishell);
	else
		ret = 1;
	free(cmd);
	free(argv);
	free_commands(minishell->commands);
	ft_free_builtins(minishell);
	return (ret);
}
