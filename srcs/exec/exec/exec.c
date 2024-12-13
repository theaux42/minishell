/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/13 17:21:53 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(char *cmd, t_command *command, char **argv, t_minishell *minishell)
{
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
		exit(exec_builtins(command, &minishell->env, minishell->history));
	else
	{
		execve(cmd, argv, minishell->env);
		exit_error("Execve");
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
		free(argv);
		return (-1);
	}
	else if (pid == 0)
		execute_child(cmd, command, argv, minishell);
	free(argv);
	return (pid);
}

int	needs_parent_execution(char *cmd_name)
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
		status = exec_builtins(command, &minishell->env, minishell->history);
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
	int	heredoc_fd;
	t_redirection	*redir;

	redir = current->redirections;

	heredoc_fd = -1;
	while (redir)
	{
		if (redir->type == REDIRECTION_HEREDOC)
		{
			heredoc_fd = handle_heredoc(redir->file);
			if (heredoc_fd == -1)
				return ;
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(heredoc_fd);
				return ;
			}
			close(heredoc_fd);
		}
		redir = redir->next;
	}

	current->prev_pipe = *prev_fd;
	if (current->is_builtin && needs_parent_execution(current->tokens->value))
	{
		status = exec_builtins(current, &minishell->env, minishell->history);
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
	wait_for_children(minishell->commands);
}
