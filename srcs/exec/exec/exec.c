/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:27:21 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 14:37:30 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_child(char *cmd, t_command *command, char **argv, t_minishell *minishell)
{
	t_redirection *redir;
	int fd;

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

	redir = command->redirections;
	while (redir)
	{
		if (redir->type == REDIRECTION_INPUT)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == REDIRECTION_OUTPUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == REDIRECTION_APPEND)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(fd);
				exit(1);
			}
			close(fd);
		}
		else if (redir->type == REDIRECTION_HEREDOC)
		{
			if (dup2(redir->fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(redir->fd);
				exit(1);
			}
			close(redir->fd);
		}
		redir = redir->next;
	}

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

void	execute_single_command(t_minishell *minishell, t_command *current, int *prev_fd)
{
	t_redirection *redir;

	// Handle all redirections first
	redir = current->redirections;
	while (redir)
	{
		if (redir->type == REDIRECTION_HEREDOC)
		{
			if (handle_heredoc_redirection(redir) == -1)
			{
				minishell->status = 1;
				return;
			}
		}
		redir = redir->next;
	}

	// Rest of command execution
	current->prev_pipe = *prev_fd;
	current->pid = exec_cmd(minishell, current);
	if (current->pid == CMD_NOT_FOUND)
		no_cmd_handler(current);
	if (current->pid == -1)
		exit(EXIT_FAILURE);

	// Close heredoc fds in the parent
	redir = current->redirections;
	while (redir)
	{
		if (redir->type == REDIRECTION_HEREDOC)
		{
			close(redir->fd);
		}
		redir = redir->next;
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
