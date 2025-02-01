/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:42:23 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 23:53:29 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*type_str(t_token_type type)
{
	if (type == COMMAND)
		return ("COMMAND");
	if (type == OPTION)
		return ("OPTION");
	if (type == ARGUMENT)
		return ("ARGUMENT");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_INPUT)
		return ("REDIR_INPUT");
	if (type == REDIR_OUTPUT)
		return ("REDIR_OUTPUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == REDIR_HEREDOC)
		return ("REDIR_HEREDOC");
	if (type == REDIR_TARGET)
		return ("REDIR_TARGET");
	return ("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current)
	{
		printf("   Token n%i: %s - %s\n", i, current->value,
			type_str(current->type));
		current = current->next;
		i++;
	}
}

void	print_redirections(t_redirection *redirections)
{
	t_redirection	*current;
	int				i;

	current = redirections;
	i = 0;
	while (current)
	{
		printf("		Redirection n%i: %s - %s\n", i, current->file,
			type_str(current->type));
		current = current->next;
		i++;
	}
}

void	print_commands(t_command *commands)
{
	t_command	*current;
	int			i;

	current = commands;
	i = 0;
	printf("=== COMMANDS ===\n");
	while (current)
	{
		printf(" --- Command n%i ---\n", i);
		print_tokens(current->tokens);
		if (current->redirections)
		{
			printf("	--- Redirections ---\n");
			print_redirections(current->redirections);
		}
		current = current->next;
		i++;
	}
	printf("=== COMMANDS ===\n");
}
