/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 06:06:07 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 04:50:01 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	int	pid;

	pid = execution(tokens->value, command, minishell);
	return (pid);
}

int	execute_builtin_command(t_minishell *minishell, t_command *command,
		t_token *tokens)
{
	char	*cmd;
	int		pid;

	if (minishell->cmd_count == 1 && is_builtin(tokens->value))
	{
		if (parent_apply_redir(command, minishell) == -1)
			return (1);
		minishell->status = parent_builtins(command, minishell);
		return (CMD_PARENT_BUILTINS);
	}
	cmd = ft_strdup(tokens->value);
	if (!cmd)
		return (CMD_NOT_FOUND);
	pid = execution(cmd, command, minishell);
	free(cmd);
	return (pid);
}

static int	handle_no_cmd_redir(t_command *command, t_minishell *minishell)
{
	parent_apply_redir(command, minishell);
	if (command->pipes[1] != -1)
		close(command->pipes[1]);
	if (command->prev_pipe != -1)
		close(command->prev_pipe);
	if (command->pipes[0] != -1)
	{
		if (!command->next)
			close(command->pipes[0]);
	}
	command->pipes[0] = -1;
	return (CMD_PARENT_BUILTINS);
}

int	exec_cmd(t_minishell *minishell, t_command *command)
{
	t_token	*tokens;
	int		pid;

	tokens = command->tokens;
	while (tokens && tokens->type != COMMAND)
		tokens = tokens->next;
	if (!tokens)
	{
		if (command->redirections)
			return (handle_no_cmd_redir(command, minishell));
		else
			return (0);
	}
	if (!is_builtin(tokens->value))
		pid = execution(tokens->value, command, minishell);
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
	{
		(close(minishell->fds[STDIN_FILENO]),
			close(minishell->fds[STDOUT_FILENO]));
		set_signal_child();
		execute_child(cmd, command, minishell, argv);
	}
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
