/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:42:23 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/10 01:55:58 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (type == REDIRECTION_INPUT)
		return ("REDIRECTION_INPUT");
	if (type == REDIRECTION_OUTPUT)
		return ("REDIRECTION_OUTPUT");
	if (type == REDIRECTION_APPEND)
		return ("REDIRECTION_APPEND");
	if (type == REDIRECTION_HEREDOC)
		return ("REDIRECTION_HEREDOC");
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
		printf("Token n%i: %s - %s\n", i, current->value,
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
	while (current)
	{
		printf("Command n%i:\n", i);
		print_tokens(current->tokens);
		current = current->next;
		i++;
	}
}
