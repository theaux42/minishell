/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 14:46:36 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(char *cmd, t_command *command, t_minishell *minishell,
		char **argv)
{
	if (command->redirections)
		if (exec_redirections(command->redirections, minishell))
			exit_child(NULL, minishell, cmd, argv);
	(close(minishell->fds[STDIN_FILENO]), close(minishell->fds[STDOUT_FILENO]));
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			exit_child("dup2_input_fd", minishell, cmd, argv);
		close(command->prev_pipe);
	}
	if (command->pipes[1] != -1)
	{
		if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
			exit_child("dup2_output_fd", minishell, cmd, argv);
		close(command->pipes[1]);
	}
	if (command->pipes[0] != -1)
		close(command->pipes[0]);
	if (is_builtin(cmd))
		exit(child_builtins(argv, cmd, command, minishell) % 256);
	else
	{
		execve(cmd, argv, minishell->env);
		exit_child(ERR_EXECVE, minishell, cmd, argv);
	}
}

static void	close_fds(int *prev_fd, t_command *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->pipes[1] != -1)
		close(current->pipes[1]);
	*prev_fd = current->pipes[0];
}

static void	execute_single_command(t_minishell *minishell, t_command *current,
		int *prev_fd)
{
	minishell->fds[STDIN_FILENO] = dup(STDIN_FILENO);
	minishell->fds[STDOUT_FILENO] = dup(STDOUT_FILENO);
	init_pipes(current, minishell);
	current->prev_pipe = *prev_fd;
	current->pid = exec_cmd(minishell, current);
	if (current->pid == -1)
		exit_parent("", minishell, true);
	close_fds(prev_fd, current);
	dup2(minishell->fds[STDIN_FILENO], STDIN_FILENO);
	dup2(minishell->fds[STDOUT_FILENO], STDOUT_FILENO);
	(close(minishell->fds[STDIN_FILENO]), close(minishell->fds[STDOUT_FILENO]));
}

static void	check_signal_exec(t_minishell *minishell)
{
	if (minishell->status == 128 + SIGINT)
		printf("\n");
	else if (minishell->status == 128 + SIGQUIT)
		printf(MSG_COREDUMP);
}

void	execute_commands(t_minishell *minishell)
{
	t_command	*current;
	t_command	*next;
	int			prev_fd;

	prev_fd = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	minishell->cmd_count = ft_cmd_count(minishell->commands);
	current = minishell->commands;
	while (current)
	{
		next = current->next;
		execute_single_command(minishell, current, &prev_fd);
		current = next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(minishell);
	check_signal_exec(minishell);
	current = minishell->commands;
	free_commands(current);
}
