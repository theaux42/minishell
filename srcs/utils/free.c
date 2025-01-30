/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 23:36:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 14:54:33 by tbabou           ###   ########.fr       */
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
		if (current->file)
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

	if (!commands)
		return ;
	current = commands;
	while (current)
	{
		next = current->next;
		if (current->tokens)
		{
			free_tokens(current->tokens);
			current->tokens = NULL;
		}
		if (current->redirections)
		{
			free_redirections(current->redirections);
			current->redirections = NULL;
		}
		if (current)
			free(current);
		current = next;
	}
}

void	ft_free_builtins(t_minishell *minishell)
{
	if (minishell->fds[STDIN_FILENO] != -1)
		close(minishell->fds[STDIN_FILENO]);
	if (minishell->fds[STDOUT_FILENO] != -1)
		close(minishell->fds[STDOUT_FILENO]);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
}
