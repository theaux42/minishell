/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 03:14:54 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_redirection *redirections)
{
	int	fd;

	while (redirections)
	{
		if (redirections->type == REDIR_INPUT)
			fd = open(redirections->file, O_RDONLY);
		else if (redirections->type == REDIR_OUTPUT)
			fd = open(redirections->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirections->type == REDIR_APPEND)
			fd = open(redirections->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redirections->type == REDIR_HEREDOC)
			fd = open(redirections->file, O_RDONLY);
		if (fd == -1)
			exit_error("Error opening file");
		if (redirections->type == REDIR_INPUT)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirections = redirections->next;
	}
}
