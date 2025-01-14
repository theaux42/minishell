/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:25:30 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/14 09:47:57 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(char *line, int *i, char *new_line,
		t_minishell *minishell)
{
	char	*current_key;
	char	*value;
	char	*temp;

	(*i)++;
	current_key = get_current_key(line, i);
	if (!current_key)
	{
		new_line = ft_addchar(new_line, '$');
		return (new_line);
	}
	value = get_env_var(current_key, minishell);
	free(current_key);
	if (!value)
		return (NULL);
	temp = ft_strjoin(new_line, value);
	free(new_line);
	free(value);
	if (!temp)
		return (NULL);
	return (temp);
}

char	*expand_line(char *line, t_minishell *minishell)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = ft_calloc(1, 1);
	if (!new_line)
		return (NULL);
	while (line[i] && line[i] != '\0')
	{
		if (line[i] == '$')
		{
			new_line = expand_env_var(line, &i, new_line, minishell);
			if (!new_line)
				return (NULL);
		}
		else
		{
			new_line = ft_addchar(new_line, line[i++]);
			if (!new_line)
				return (NULL);
		}
	}
	return (new_line);
}

void	process_token(t_token *token, t_minishell *minishell)
{
	char	*temp;

	if (need_expansion(token->value))
	{
		temp = token->value;
		token->value = expand_line(token->value, minishell);
		if (!token->value)
			exit_error("expand_line");
		free(temp);
	}
	temp = token->value;
	token->value = process_quote(token->value);
	if (!token->value)
		exit_error("process_quote");
	free(temp);
}

void	expand_tokens(t_token *tokens, t_minishell *minishell)
{
	t_token	*current_token;

	current_token = tokens;
	while (current_token)
	{
		process_token(current_token, minishell);
		current_token = current_token->next;
	}
}

bool	check_commands(t_command *commands)
{
	t_command	*current_command;
	t_token		*current_token;

	current_command = commands;
	while (current_command)
	{
		current_token = current_command->tokens;
		while (current_token)
		{
			if (check_missused_quotes(current_token->value))
				return (false);
			current_token = current_token->next;
		}
		current_command = current_command->next;
	}
	return (true);
}

bool	expand_commands(t_command *commands, t_minishell *minishell)
{
	t_command	*current;

	current = commands;
	if (!check_commands(commands))
	{
		ft_dprintf(2, ERR_UNCLOSED_QUOTES);
		return (false);
	}
	while (current)
	{
		expand_tokens(current->tokens, minishell);
		current = current->next;
	}
	return (true);
}
