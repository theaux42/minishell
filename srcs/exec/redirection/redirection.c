/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/20 21:27:14 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_line_heredoc(char *delimiter)
{
	char	*line;
	char	*heredoc;

	heredoc = ft_strdup("");
	while (1)
	{
		line = readline(HEREDOC_PROMPT);
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		heredoc = ft_strjoin_double(heredoc, line, "\n");
		if (!heredoc)
			return (NULL);
		free(line);
	}
	return (heredoc);
}

int	handle_heredoc(char *delimiter, t_minishell *minishell)
{
	int		pipe_fd[2];
	bool	need_to_expand;
	char	*line;

	if (pipe(pipe_fd) == -1)
		exit_parent(ERR_PIPE_FAIL, minishell);
	need_to_expand = true;
	if (ft_edgecmp(delimiter, '"') || ft_edgecmp(delimiter, '\''))
		need_to_expand = false;
	line = get_line_heredoc(delimiter);
	if (!line)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	if (need_to_expand)
		line = expand_line(line, minishell);
	ft_putstr_fd(line, pipe_fd[1]);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	apply_redirections(t_redirection *redirections, t_minishell *minishell)
{
	int	fd;
	printf("apply_redirections\n");
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
			return (ft_dprintf(2, ERR_NO_RIGHT, redirections->file), 1);
		if (redirections->type == REDIR_INPUT
			|| redirections->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirections = redirections->next;
	}
	return (0);
}
