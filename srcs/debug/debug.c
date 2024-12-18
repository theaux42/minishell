/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 16:42:23 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 03:09:17 by tbabou           ###   ########.fr       */
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

void	print_tokens2(t_token *tokens, char separator)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("%s%c", current->value, separator);
		current = current->next;
	}
	printf("\n");
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

void	print_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
	{
		fprintf(stderr, "env: No environment variables found\n");
		return ;
	}
	while (env[i])
		i++;
}
