/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 22:41:56 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/28 22:37:43 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_new_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->tokens = NULL;
	new_command->redirections = NULL;
	new_command->pipes[0] = -1;
	new_command->pipes[1] = -1;
	new_command->prev_pipe = -1;
	new_command->is_absolute = 0;
	new_command->is_last = 0;
	new_command->pid = 0;
	new_command->next = NULL;
	return (new_command);
}

t_token	*init_new_token(void)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = NULL;
	new_token->type = COMMAND;
	new_token->next = NULL;
	return (new_token);
}

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

static int fake_ft_strcmp(char *s1, char *s2)
{
    int	i;

    if (!s1 && !s2)
        return (-1337);
    if (!s1)
        return (-1337);
    if (!s2)
        return (-1337);
    i = 0;
    if (s1[0] == '\0' && s2[0] == '\0')
        return (0);
    while (s1[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static bool	parse_commands(t_command **head, char **commands)
{
	t_command	*current;
	t_command	*new_cmd;
	int			i;
	int			j;

	i = 0;
	j = 0;
	*head = init_new_command();
	if (!*head)
		return (false);
	current = *head;
	while (commands[i])
	{
		if (!commands[i] || fake_ft_strcmp(commands[i], "") == -1337)
			return ((free(commands), ft_dprintf(2, ERR_MALLOC)), false);
		if (fake_ft_strcmp(commands[i], "|") == 0)
		{
			j = -1;
			new_cmd = init_new_command();
			if (!new_cmd)
				return (false);
			current->next = new_cmd;
			current = new_cmd;
		}
		else
			if (add_tokens(&current->tokens, commands[i], j))
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
	if (!parse_commands(&head, commands))
		return (free_commands(head), NULL);
	ft_freesplit(commands);
	if (!parse_redirections(head, minishell))
		return (free_commands(head), NULL);
	if (!validate_commands(head, minishell))
		return (free_commands(head), NULL);
	if (!expand_commands(head, minishell))
		return (free_commands(head), NULL);
	return (head);
}
