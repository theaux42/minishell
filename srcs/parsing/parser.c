/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 21:31:38 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	add_tokens(t_token **head, char *token, int position)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (true);
	new_token->value = ft_strdup(token);
	if (!new_token->value)
	{
		free(new_token);
		return (true);
	}
	new_token->type = get_tokens_type(token, position);
	new_token->next = NULL;
	if (*head == NULL)
		*head = new_token;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (false);
}

static bool	handle_pipe(t_command **current, int *command_nbr)
{
	t_command	*new_cmd;

	new_cmd = init_new_command();
	if (!new_cmd)
		return (false);
	(*current)->next = new_cmd;
	*current = new_cmd;
	*command_nbr = -1;
	return (true);
}

static bool	parse_commands(t_command **head, char **commands)
{
	t_command	*current;
	int			i;
	int			j;

	*head = init_new_command();
	if (!*head || !commands)
		return (false);
	current = *head;
	i = 0;
	j = 0;
	while (commands[i])
	{
		if (!commands[i] || ft_safecmp(commands[i], "") == -4242)
			return ((free(commands), ft_dprintf(2, ERR_MALLOC)), false);
		if (ft_safecmp(commands[i], "|") == 0)
		{
			if (!handle_pipe(&current, &j))
				return (false);
		}
		else if (add_tokens(&current->tokens, commands[i], j))
			return (false);
		i++;
		j++;
	}
	return (true);
}

t_command	*get_commands(char *line, t_minishell *minishell)
{
	t_command	*head;
	char		**commands;

	head = NULL;
	if (line[0] == '\0')
		return (NULL);
	commands = ft_ms_split(line);
	if (!commands)
		return (NULL);
	if (!check_empty_pipes(commands))
		return (ft_freesplit(commands), NULL);
	if (!parse_commands(&head, commands))
		return (free_commands(head), NULL);
	ft_freesplit(commands);
	minishell->cmd_count = ft_cmd_count(head);
	if (!parse_redirections(head, minishell))
		return (free_commands(head), NULL);
	if (!validate_commands(head, minishell))
		return (free_commands(head), NULL);
	if (!expand_commands(head, minishell))
		return (free_commands(head), NULL);
	if (DEBUG_MODE)
		print_commands(minishell->commands);
	return (head);
}
