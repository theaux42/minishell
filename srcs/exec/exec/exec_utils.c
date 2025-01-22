/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:42:54 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/22 10:21:50 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(t_command *commands, t_minishell *minishell)
{
	if (commands->next)
	{
		if (pipe(commands->pipes) == -1)
			exit_parent(ERR_PIPE_FAIL, minishell, true);
	}
	else
	{
		commands->pipes[0] = -1;
		commands->pipes[1] = -1;
	}
}

void	wait_for_children(t_minishell *minishell)
{
	t_command	*cmd;

	cmd = minishell->commands;
	while (cmd)
	{
		if (cmd->pid == CMD_NO_RIGHT || cmd->pid == CMD_NOT_FOUND)
		{
			minishell->status = 126 % 256;
			if (cmd->pid == CMD_NOT_FOUND)
				minishell->status = 127 % 256;
			cmd = cmd->next;
		}
		else
		{
			waitpid(cmd->pid, &minishell->status, 0);
			if (WIFEXITED(minishell->status))
				minishell->status = WEXITSTATUS(minishell->status) % 256;
			else if (WIFSIGNALED(minishell->status))
				minishell->status = 128 + WTERMSIG(minishell->status) % 256;
			else if (WIFSTOPPED(minishell->status))
				minishell->status = 128 + WSTOPSIG(minishell->status) % 256;
			cmd = cmd->next;
		}
	}
}

int	count_arguments(t_command *command)
{
	int		argc;
	t_token	*token;

	argc = 0;
	token = command->tokens;
	while (token)
	{
		if (token->type == REDIR_APPEND || token->type == REDIR_HEREDOC
			|| token->type == REDIR_INPUT || token->type == REDIR_OUTPUT)
		{
			token = token->next;
			if (token)
				token = token->next;
			continue ;
		}
		if (token->type == COMMAND || token->type == ARGUMENT)
			argc++;
		token = token->next;
	}
	return (argc);
}

int	fill_arguments(char **argv, t_command *command)
{
	t_token	*token;
	int		i;

	token = command->tokens;
	i = 0;
	while (token)
	{
		if (token->type == COMMAND || token->type == ARGUMENT)
			argv[i++] = token->value;
		if (token->type == REDIR_APPEND || token->type == REDIR_INPUT
			|| token->type == REDIR_OUTPUT)
			break ;
		token = token->next;
	}
	argv[i] = NULL;
	return (0);
}

void	cmd_error_handler(t_command *current, t_minishell *minishell,
		int status)
{
	t_token	*tok;

	(void)minishell;
	tok = current->tokens;
	while (tok && tok->type != COMMAND)
		tok = tok->next;
	if (status == CMD_NOT_FOUND)
	{
		ft_dprintf(2, ERR_CMD_NOT_FOUND, current->tokens->value);
		minishell->status = 127 % 256;
	}
	else if (status == CMD_NO_RIGHT)
	{
		ft_dprintf(2, ERR_NO_RIGHT, current->tokens->value);
		minishell->status = 126 % 256;
	}
}
