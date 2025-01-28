/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 16:19:24 by tbabou           ###   ########.fr       */
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

int exec_redirections(t_redirection *redirections, t_minishell *minishell)
{
    int fd;

    while (redirections)
    {
        if (redirections->type == REDIR_INPUT)
            fd = open(redirections->file, O_RDONLY);
        else if (redirections->type == REDIR_OUTPUT)
            fd = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (redirections->type == REDIR_APPEND)
            fd = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (redirections->type == REDIR_HEREDOC)
            fd = handle_heredoc(redirections->file, minishell);

        if (fd == -1)
        {
            ft_dprintf(2, ERR_NO_RIGHT, redirections->file);
            return (1);
        }

        if (redirections->type == REDIR_INPUT || redirections->type == REDIR_HEREDOC)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
                return (close(fd), 1);
        }
        else
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
                return (close(fd), 1);
        }

        // Ferme immédiatement le FD après duplication
        close(fd);
        redirections = redirections->next;
    }
    return (0);
}


void	apply_redirections(t_command *command, t_minishell *minishell)
{
	if (!command->redirections)
		return ;
	if (exec_redirections(command->redirections, minishell))
		exit_parent(NULL, minishell, true);
	if (command->prev_pipe != -1)
	{
		if (dup2(command->prev_pipe, STDIN_FILENO) == -1)
			exit_parent("dup2_input_fd", minishell, true);
		close(command->prev_pipe);
	}
	if (command->pipes[1] != -1)
	{
		if (dup2(command->pipes[1], STDOUT_FILENO) == -1)
			exit_parent("dup2_output_fd", minishell, true);
		close(command->pipes[1]);
	}
	if (command->pipes[0] != -1)
		close(command->pipes[0]);
}
