/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 21:23:11 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 21:33:10 by tbabou           ###   ########.fr       */
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

t_token	*create_arg_token(char *value, t_token *next)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = ARGUMENT;
	new->next = next;
	return (new);
}

int	split_expanded_token(t_token *token, char **split)
{
	t_token	*current;
	int		i;

	if (!split[0] || !split[1])
		return (0);
	free(token->value);
	token->value = ft_strdup(split[0]);
	if (!token->value)
		return (-1);
	current = token;
	i = 1;
	while (split[i])
	{
		current->next = create_arg_token(split[i], current->next);
		if (!current->next)
			return (-1);
		current = current->next;
		i++;
	}
	return (0);
}
