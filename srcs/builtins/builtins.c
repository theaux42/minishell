/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:54:09 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 13:56:23 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	is_valid_args(t_token *tokens, char *cmd)
{
	if (ft_strncmp(cmd, "cd", 2) == 0 && ft_cmdlen(tokens) > 2)
		return (ft_dprintf(2, ERR_TOO_MANY_ARGS, cmd));
	return (true);
}

static int	builtins(t_command *command, char ***env, t_minishell *minishell,
		bool msg)
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
		ret = ft_env(*env, false);
	return (ret);
}

int	parent_builtins(t_command *command, t_minishell *minishell)
{
	int	ret;

	ret = 0;
	if (DEBUG_MODE)
		printf(DEBUG_EXEC_PARENT);
	if (is_valid_args(command->tokens, command->tokens->value))
		ret = builtins(command, &minishell->env, minishell, true);
	else
		ret = 1;
	return (ret);
}

int	child_builtins(char **argv, char *cmd, t_command *command,
		t_minishell *minishell)
{
	int	ret;

	(void)cmd;
	ret = 0;
	if (DEBUG_MODE)
		printf(DEBUG_EXEC_CHILD);
	if (is_valid_args(command->tokens, command->tokens->value))
		ret = builtins(command, &minishell->env, minishell, false);
	else
		ret = 1;
	free(cmd);
	free(argv);
	free_commands(minishell->commands);
	ft_free_builtins(minishell);
	return (ret);
}
