/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/13 15:55:36 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(char *cmd, t_command *command, t_minishell *minishell,
		char **argv)
{
	if (command->redirections)
		apply_redirections(command->redirections, minishell);
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			exit_error_child("dup2_input_fd", minishell, cmd, argv);
		close(command->prev_pipe);
	}
	if (command->pipes[1] != -1)
	{
		if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
			exit_error_child("dup2_output_fd", minishell, cmd, argv);
		close(command->pipes[1]);
	}
	if (command->pipes[0] != -1)
		close(command->pipes[0]);
	if (command->is_builtin)
		exit(exec_builtins_2(argv, cmd, command, minishell));
	else
	{
		execve(cmd, argv, minishell->env);
		exit_error_child("execve", minishell, cmd, argv);
	}
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

void	close_fds(int *prev_fd, t_command *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->pipes[1] != -1)
		close(current->pipes[1]);
	*prev_fd = current->pipes[0];
}

void	execute_single_command(t_minishell *minishell, t_command *current,
		int *prev_fd)
{
	bool	exit_fork;

	exit_fork = false;
	current->prev_pipe = *prev_fd;
	if (current->is_builtin && ft_strcmp(current->tokens->value, "exit") == 0
		&& (current->pipes[1] != -1 || current->prev_pipe != -1))
		exit_fork = true;
	if (current->is_builtin && needs_parent_execution(current->tokens->value)
		&& !exit_fork)
	{
		minishell->status = exec_builtins(current, minishell);
		current->pid = 0;
	}
	else
	{
		current->pid = exec_cmd(minishell, current, exit_fork);
		if (current->pid == CMD_NOT_FOUND)
			no_cmd_handler(current);
		if (current->pid == -1)
			exit(EXIT_FAILURE);
	}
	close_fds(prev_fd, current);
}

void	check_signal_exec(t_minishell *minishell)
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
	init_pipes(minishell->commands, minishell);
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
	while (current)
	{
		next = current->next;
		free_command(current);
		current = next;
	}
}
