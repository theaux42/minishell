/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:54:09 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/21 12:35:49 by tbabou           ###   ########.fr       */
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
	if (ft_strcmp(str, "echo") == 0)
		return (1);
	else if (ft_strcmp(str, "cd") == 0)
		return (1);
	else if (ft_strcmp(str, "pwd") == 0)
		return (1);
	else if (ft_strcmp(str, "export") == 0)
		return (1);
	else if (ft_strcmp(str, "unset") == 0)
		return (1);
	else if (ft_strcmp(str, "env") == 0)
		return (1);
	else if (ft_strcmp(str, "exit") == 0)
		return (1);
	return (0);
}

bool	is_valid_args(t_token *tokens, char *cmd, bool print_exit)
{
	bool	is_exit;

	is_exit = false;
	if (ft_strncmp(cmd, "exit", 4) == 0)
		is_exit = true;
	if (tokens->next != NULL && tokens->next->value != NULL
		&& *tokens->next->value && !ft_isdigits_str(tokens->next->value)
		&& is_exit)
	{
		ft_dprintf(2, ERR_NUM_ARG, cmd);
		return (false);
	}
	if (ft_strncmp(cmd, "cd", 2) == 0 || ft_strncmp(cmd, "exit", 4) == 0)
	{
		if (ft_cmdlen(tokens) > 2)
		{
			if (is_exit && print_exit)
				printf("exit\n");
			ft_dprintf(2, ERR_TOO_MANY_ARGS, cmd);
			return (false);
		}
	}
	return (true);
}

int	builtins(t_command *command, char ***env, t_minishell *minishell, bool msg)
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
		ret = ft_exit(command->tokens->next, minishell, msg);
	else if (ft_strncmp(command->tokens->value, "export", 6) == 0)
		ret = ft_export(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "unset", 5) == 0)
		ret = ft_unset(command->tokens->next, env);
	else if (ft_strncmp(command->tokens->value, "env", 3) == 0)
		ret = ft_env(*env);
	return (ret);
}

int	parent_builtins(t_command *command, t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (DEBUG_MODE)
		printf(DEBUG_EXEC_PARENT);
	if (is_valid_args(command->tokens, command->tokens->value, true))
		ret = builtins(command, &minishell->env, minishell, true) % 256;
	else
		ret = 1 % 256;
	return (ret);
}

int	child_builtins(char **argv, char *cmd, t_command *command,
		t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (DEBUG_MODE)
		printf(DEBUG_EXEC_CHILD);
	if (is_valid_args(command->tokens, command->tokens->value, true))
		ret = builtins(command, &minishell->env, minishell, false) % 256;
	else
		ret = 1 % 256;
	free(cmd);
	free(argv);
	free_commands(minishell->commands);
	ft_free_builtins(minishell);
	return (ret);
}
