/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 02:40:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 21:12:53 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*add_redirection(t_redirection *head, t_token_type type,
		char *file)
{
	t_redirection	*new;
	t_redirection	*current;

	new = malloc(sizeof(t_redirection));
	if (!new)
		exit_error("malloc error");
	new->type = type;
	new->file = ft_strdup(file);
	new->next = NULL;
	if (!head)
		return (new);
	current = head;
	while (current->next)
		current = current->next;
	current->next = new;
	return (head);
}

t_token	*remove_redirection_tokens(t_token *current, t_token **prev,
		t_command *command)
{
	t_token	*to_free;

	to_free = current->next;
	if (*prev)
		(*prev)->next = current->next->next;
	else
		command->tokens = current->next->next;
	free(current->value);
	free(current);
	current = to_free->next;
	free(to_free->value);
	free(to_free);
	return (current);
}

void	parse_cmd_redirections(t_command *command)
{
	t_token			*prev;
	t_token			*current;
	t_redirection	*redirections;

	prev = NULL;
	current = command->tokens;
	redirections = NULL;
	while (current)
	{
		if (current->type == REDIR_INPUT || current->type == REDIR_OUTPUT
			|| current->type == REDIR_APPEND || current->type == REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != ARGUMENT)
				exit_error("Syntax error: invalid redirection");
			redirections = add_redirection(redirections, current->type,
					current->next->value);
			current = remove_redirection_tokens(current, &prev, command);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	command->redirections = redirections;
}

void	parse_redirections(t_command *commands)
{
	t_command	*current_command;

	current_command = commands;
	while (current_command)
	{
		parse_cmd_redirections(current_command);
		current_command = current_command->next;
	}
}
