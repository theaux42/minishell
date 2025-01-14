/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 02:40:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/14 09:48:20 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*add_redirection(t_redirection *head, t_token_type type,
		char *file)
{
	t_redirection	*new;
	t_redirection	*current;

	if (!file)
		return (head);
	new = malloc(sizeof(t_redirection));
	if (!new)
		return (ft_dprintf(2, ERR_MALLOC), NULL);
	new->type = type;
	new->file = ft_strdup(file);
	if (!new->file)
	{
		free(new);
		return (ft_dprintf(2, ERR_MALLOC), NULL);
	}
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
		t_command *command, bool remove_next)
{
	t_token	*to_free;

	if (remove_next && current->next)
	{
		to_free = current->next;
		if (*prev)
			(*prev)->next = current->next->next;
		else
			command->tokens = current->next->next;
		free_token(current);
		current = to_free->next;
		free_token(to_free);
	}
	else
	{
		if (*prev)
			(*prev)->next = current->next;
		else
			command->tokens = current->next;
		to_free = current->next;
		free_token(current);
		current = to_free;
	}
	return (current);
}

bool	handle_redirection(t_token **current, t_token **prev,
		t_command *command, t_redirection **redirections)
{
	if ((*current)->type != REDIR_HEREDOC && (!(*current)->next
			|| (*current)->next->type != ARGUMENT))
		return (false);
	if ((*current)->type == REDIR_HEREDOC)
	{
		if (!validate_heredoc_delimiter((*current)->value + 2))
			return (false);
		*redirections = add_redirection(*redirections, (*current)->type,
				(*current)->value + 2);
		*current = remove_redirection_tokens(*current, prev, command, false);
	}
	else
	{
		*redirections = add_redirection(*redirections, (*current)->type,
				(*current)->next->value);
		*current = remove_redirection_tokens(*current, prev, command, true);
	}
	return (true);
}

bool	parse_cmd_redirections(t_command *command)
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
			if (!handle_redirection(&current, &prev, command, &redirections))
				return (false);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	command->redirections = redirections;
	return (true);
}

bool	parse_redirections(t_command *commands)
{
	t_command	*current_command;

	current_command = commands;
	while (current_command)
	{
		if (!parse_cmd_redirections(current_command))
			return (ft_dprintf(2, ERR_BAD_REDIRECTION), false);
		current_command = current_command->next;
	}
	return (true);
}
