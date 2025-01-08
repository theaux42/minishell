/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:42:54 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 09:28:09 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(t_command *commands, t_minishell *minishell)
{
	t_command	*current;

	current = commands;
	while (current)
	{
		if (current->next)
		{
			if (pipe(current->pipes) == -1)
				exit_error_parent(ERR_PIPE_FAIL, minishell);
		}
		else
		{
			current->pipes[0] = -1;
			current->pipes[1] = -1;
		}
		current = current->next;
	}
}

void	wait_for_children(t_minishell *minishell)
{
	t_command	*current;
	int			status;

	current = minishell->commands;
	while (current)
	{
		if (waitpid(current->pid, &status, 0) == -1)
			;
		else if (WIFEXITED(status))
			minishell->status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
			{
				ft_printf("\n");
				minishell->status = WTERMSIG(status);
			}
			else if (WTERMSIG(status) == SIGQUIT)
			{
				ft_printf("Quit (core dumped)\n");
				minishell->status = WTERMSIG(status);
			}
		}
		current = current->next;
	}
	g_signal = 0;
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

void	no_cmd_handler(t_command *current)
{
	t_token	*tok;

	tok = current->tokens;
	while (tok && tok->type != COMMAND)
		tok = tok->next;
	if (tok)
		printf(ERR_CMD_NOT_FOUND, tok->value);
	else
		printf(ERR_EMPTY_CMD);
}
