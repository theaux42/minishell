/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 06:06:07 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/18 19:26:04 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	needs_parent_execution(char *cmd_name)
{
	return (ft_strcmp(cmd_name, "cd") == 0 || ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0 || ft_strcmp(cmd_name,
			"exit") == 0);
}

int	execute_external_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	char	*cmd;
	int		pid;

	cmd = get_full_cmd(tokens->value, minishell->env);
	if (!cmd)
		return (CMD_NOT_FOUND);
	pid = execution(cmd, command, minishell);
	free(cmd);
	return (pid);
}

int	execute_builtin_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	char	*cmd;
	int		pid;
	int		status;

	if (needs_parent_execution(tokens->value))
	{
		status = exec_builtins(command, &minishell->env);
		minishell->status = status;
		return (0);
	}
	else
	{
		cmd = ft_strdup(tokens->value);
		if (!cmd)
			return (CMD_NOT_FOUND);
		pid = execution(cmd, command, minishell);
		free(cmd);
		return (pid);
	}
}

int	exec_cmd(t_minishell *minishell, t_command *command)
{
	t_token	*tokens;
	int		pid;

	tokens = command->tokens;
	while (tokens && tokens->type != COMMAND)
		tokens = tokens->next;
	if (!tokens)
		return (CMD_NOT_FOUND);
	if (!command->is_builtin)
		pid = execute_external_command(minishell, command, tokens);
	else
		pid = execute_builtin_command(minishell, command, tokens);
	return (pid);
}
