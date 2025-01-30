/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 23:41:42 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	has_redirections(t_redirection *redirections, t_token_type types)
{
	while (redirections)
	{
		if (redirections->type & types)
			return (true);
		redirections = redirections->next;
	}
	return (false);
}

static int	handle_heredoc(char *content, t_minishell *minishell)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		exit_parent(ERR_PIPE_FAIL, minishell, true);
	ft_dprintf(pipe_fd[1], "%s", content);
	close(pipe_fd[1]);
	free(content);
	return (pipe_fd[0]);
}

int	exec_redirections(t_redirection *redirs, t_minishell *minishell)
{
	int	fd;

	// if (access(redirs->file, F_OK) == -1)
	// if (redirs->type != REDIR_HEREDOC && access(redirs->file, F_OK) == -1)
	// 	return (ft_dprintf(2, ERR_NO_FILE, redirs->file), 1);
	// if (redirs->type != REDIR_HEREDOC && access(redirs->file, W_OK | R_OK) == -1)
	// 	return (ft_dprintf(2, ERR_NO_RIGHT, redirs->file), 1);
	// if (redirs->type != REDIR_HEREDOC && ft_isfolder(redirs->file))
	// 	return (ft_dprintf(2, ERR_IS_FOLDER, redirs->file), 1);
	while (redirs)
	{
		if (redirs->type == REDIR_INPUT)
		{
			fd = open(redirs->file, O_RDONLY);
			if (fd == -1)
				return (ft_dprintf(2, ERR_NO_RIGHT, redirs->file), 1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), 1);
			close(fd);
		}
		else if (redirs->type == REDIR_OUTPUT)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (ft_dprintf(2, ERR_NO_RIGHT, redirs->file), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), 1);
			close(fd);
		}
		else if (redirs->type == REDIR_APPEND)
		{
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (ft_dprintf(2, ERR_NO_RIGHT, redirs->file), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), 1);
			close(fd);
		}
		else if (redirs->type == REDIR_HEREDOC)
		{
			fd = handle_heredoc(redirs->file, minishell);
			if (fd == -1)
				return (1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), 1);
			close(fd);
		}
		redirs = redirs->next;
	}
	return (0);
}

int	parent_apply_redir(t_command *command, t_minishell *minishell)
{
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			return(-1);
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
