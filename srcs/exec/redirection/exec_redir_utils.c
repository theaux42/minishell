/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 21:14:28 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 23:51:30 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(int *prev_fd, t_command *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->pipes[1] != -1)
		close(current->pipes[1]);
	*prev_fd = current->pipes[0];
}

int	handle_no_cmd_redir(t_command *command, t_minishell *minishell)
{
	parent_redir(command, minishell);
	if (command->pipes[1] != -1)
		close(command->pipes[1]);
	if (command->prev_pipe != -1)
		close(command->prev_pipe);
	if (command->pipes[0] != -1)
	{
		if (!command->next)
			close(command->pipes[0]);
	}
	command->pipes[0] = -1;
	return (CMD_PARENT_BUILTINS);
}
