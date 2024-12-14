/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:42:54 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 16:58:49 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_pipes(t_command *commands)
{
	t_command	*current;

	current = commands;
	while (current)
	{
		if (current->next)
		{
			if (pipe(current->pipes) == -1)
				exit_error("pipe");
		}
		else
		{
			current->pipes[0] = -1;
			current->pipes[1] = -1;
		}
		current = current->next;
	}
}

void	wait_for_children(t_command *commands)
{
	t_command	*current;
	int			status;

	current = commands;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		current = current->next;
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
		if (token->type == REDIRECTION_APPEND || token->type == REDIRECTION_HEREDOC
			|| token->type == REDIRECTION_INPUT || token->type == REDIRECTION_OUTPUT)
		{
			token = token->next;
			if (token)
				token = token->next;
			continue;
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
		if (token->type == REDIRECTION_APPEND || token->type == REDIRECTION_HEREDOC
			|| token->type == REDIRECTION_INPUT || token->type == REDIRECTION_OUTPUT)
		{
			token = token->next;
			if (token)
				token = token->next;
			continue;
		}
		if (token->type == COMMAND || token->type == ARGUMENT)
		{
			argv[i] = token->value;
			i++;
		}
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
		printf("Command not found: %s\n", tok->value);
	else
		printf("Command not found\n");
}
