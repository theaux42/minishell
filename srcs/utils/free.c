/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 23:36:05 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/23 06:45:29 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	if (token->value)
		free(token->value);
	free(token);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free_token(current);
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

void	free_in_builtins(t_minishell *minishell)
{
	free(minishell->line);
	free_commands(minishell->commands);
	ft_freesplit(minishell->env);
	free(minishell);
}
