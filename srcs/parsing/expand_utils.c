/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 00:14:52 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 13:23:19 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_var(char *key, t_minishell *minishell)
{
	char	*value;
	char	*temp;

	if (key && isdigit(key[0]))
		value = ft_substr(key, 1, ft_strlen(key) - 1);
	else if (key && key[0] == '?')
	{
		temp = ft_itoa(minishell->status);
		if (!temp)
			return (NULL);
		value = ft_strdup(temp);
		free(temp);
	}
	else
	{
		temp = get_env(key, minishell->env);
		if (temp)
			value = ft_strdup(temp);
		else
			value = ft_strdup("");
	}
	if (!value)
		return (NULL);
	return (value);
}

char	*get_current_key(char *line, int *i)
{
	int		start;
	int		len;
	char	*current_key;

	start = *i;
	if (!get_active_quotes(line, start - 1) && (line[start] == '\''
			|| line[start + 1] == '\"'))
		return (ft_strdup(""));
	if (line[start] == '?')
	{
		(*i)++;
		current_key = ft_substr(line, start, 1);
		return (current_key);
	}
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	len = *i - start;
	current_key = ft_substr(line, start, len);
	if (!current_key)
		return (NULL);
	if (!is_valid_key(current_key))
		return (free(current_key), NULL);
	return (current_key);
}

bool	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!key)
		return (false);
	if (ft_isdigit(key[i]))
		return (false);
	if (key[i] == '_' || ft_isalpha(key[i]))
	{
		i++;
		while (key[i])
		{
			if (key[i] == '_' || ft_isalnum(key[i]))
				i++;
			else
				return (false);
		}
		return (true);
	}
	return (false);
}

bool	need_expansion(char *value)
{
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;

	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (value[i])
	{
		if (value[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (value[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (value[i] == '$' && !in_single_quote)
			return (true);
		i++;
	}
	return (false);
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
