/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 23:36:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 07:21:18 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		if (current)
			free(current);
		current = next;
	}
}

void	free_redirections(t_redirection *redirections)
{
	t_redirection	*current;
	t_redirection	*next;

	current = redirections;
	while (current)
	{
		if (current->file && *current->file)
			free(current->file);
		next = current->next;
		free(current);
		current = next;
	}
}

void	free_commands(t_command *commands)
{
	t_command	*current;
	t_command	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		if (current->tokens && current->is_builtin == 0)
			free_tokens(current->tokens);
		if (current->redirections)
			free_redirections(current->redirections);
		free(current);
		current = next;
	}
}
