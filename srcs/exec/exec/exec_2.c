/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 06:06:07 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/24 10:57:35 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	char	*cmd;
	int		pid;

	cmd = get_full_cmd(tokens->value, minishell->env);
	if (!cmd)
		return (ft_dprintf(2, ERR_CMD_NOT_FOUND, tokens->value), CMD_NOT_FOUND);
	if (access(cmd, F_OK) != 0 || access(cmd, X_OK) != 0)
	{
		ft_dprintf(2, ERR_NO_RIGHT, cmd);
		return (free(cmd), CMD_NO_RIGHT);
	}
	pid = execution(cmd, command, minishell);
	free(cmd);
	return (pid);
}

int	execute_builtin_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	char	*cmd;
	int		pid;

	if (minishell->cmd_count == 1 && is_builtin(tokens->value))
	{
		apply_redirections(command, minishell);
		minishell->status = parent_builtins(command, minishell);
		return (minishell->status);
	
	}
	cmd = ft_strdup(tokens->value);
	if (!cmd)
		return (CMD_NOT_FOUND);
	pid = execution(cmd, command, minishell);
	free(cmd);
	return (pid);
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
	if (!is_builtin(tokens->value))
		pid = execute_external_command(minishell, command, tokens);
	else
		pid = execute_builtin_command(minishell, command, tokens);
	return (pid);
}

int	execution(char *cmd, t_command *command, t_minishell *minishell)
{
	pid_t	pid;
	char	**argv;
	int		argc;

	argc = count_arguments(command);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (-1);
	fill_arguments(argv, command);
	pid = fork();
	if (pid < 0)
	{
		ft_freesplit(argv);
		return (-1);
	}
	else if (pid == 0)
		execute_child(cmd, command, minishell, argv);
	free(argv);
	return (pid);
}

int	ft_cmd_count(t_command *commands)
{
	int			count;
	t_command	*current;

	if (!commands)
		return (0);
	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
