/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/02/01 00:23:11 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_heredoc_exec(char *content, t_minishell *minishell)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		exit_parent(ERR_PIPE_FAIL, minishell, true);
	ft_dprintf(pipe_fd[1], "%s", content);
	close(pipe_fd[1]);
	if (pipe_fd[0] == -1)
		return (false);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (close(pipe_fd[0]), false);
	close(pipe_fd[0]);
	return (true);
}

static bool	manage_redir(t_redirection *redirection)
{
	int	fd;
	int	target_fd;

	if (redirection->type == REDIR_INPUT)
		fd = open(redirection->file, O_RDONLY);
	else if (redirection->type == REDIR_OUTPUT)
		fd = open(redirection->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirection->type == REDIR_APPEND)
		fd = open(redirection->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (ft_dprintf(2, ERR_NO_RIGHT, redirection->file), false);
	if (redirection->type == REDIR_INPUT)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
		return (close(fd), false);
	close(fd);
	return (true);
}

int	exec_redirections(t_redirection *redirs, t_minishell *minishell)
{
	if (redirs->type == REDIR_INPUT && access(redirs->file, F_OK) == -1)
	{
		minishell->status = 1;
		return (ft_dprintf(2, ERR_NO_FILE, redirs->file), 1);
	}
	while (redirs)
	{
		if (redirs->type >= REDIR_INPUT && redirs->type <= REDIR_APPEND)
		{
			if (!manage_redir(redirs))
				return (1);
		}
		else if (redirs->type == REDIR_HEREDOC)
		{
			if (!handle_heredoc_exec(redirs->file, minishell))
				return (1);
		}
		redirs = redirs->next;
	}
	return (0);
}

int	parent_redir(t_command *command, t_minishell *minishell)
{
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			return (-1);
		close(command->prev_pipe);
	}
	if (command->pipes[1] != -1)
	{
		if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
			return (-1);
		close(command->pipes[1]);
	}
	if (command->pipes[0] != -1)
		close(command->pipes[0]);
	if (command->redirections)
		if (exec_redirections(command->redirections, minishell))
			return (-1);
	return (0);
}
