/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 03:20:45 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(char *cmd, t_command *command, char ***env, char **argv)
{
	if (command->redirections)
		apply_redirections(command->redirections);
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			exit_error("dup2 input_fd");
		close(command->prev_pipe);
	}
	if (command->pipes[1] != -1)
	{
		if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
			exit_error("dup2 output_fd");
		close(command->pipes[1]);
	}
	if (command->pipes[0] != -1)
		close(command->pipes[0]);
	if (command->is_builtin)
		exit(exec_builtins(command, env));
	else
	{
		execve(cmd, argv, *env);
		exit_error("Execve");
	}
}

int	execution(char *cmd, t_command *command, char ***env)
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
		free(argv);
		return (-1);
	}
	else if (pid == 0)
		execute_child(cmd, command, env, argv);
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
	int	status;

	current->prev_pipe = *prev_fd;
	if (current->is_builtin && needs_parent_execution(current->tokens->value))
	{
		status = exec_builtins(current, &minishell->env);
		minishell->status = status;
		current->pid = 0;
	}
	else
	{
		current->pid = exec_cmd(minishell, current);
		if (current->pid == CMD_NOT_FOUND)
			no_cmd_handler(current);
		if (current->pid == -1)
			exit(EXIT_FAILURE);
	}
	close_fds(prev_fd, current);
}

void	execute_commands(t_minishell *minishell)
{
	t_command	*current;
	int			prev_fd;

	prev_fd = -1;
	init_pipes(minishell->commands);
	current = minishell->commands;
	while (current)
	{
		execute_single_command(minishell, current, &prev_fd);
		current = current->next;
	}
	if (prev_fd != -1)
		close(prev_fd);
	wait_for_children(minishell);
}
