/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:33:11 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/17 15:28:44 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_token(char *line, int *i, int *is_in_arg)
{
	int	word_count;

	word_count = 1;
	if (!(*is_in_arg) && ft_isredir(line[*i]))
	{
		(*i)++;
		if (line[*i] == line[*i - 1])
			(*i)++;
	}
	else
	{
		while (line[*i] && (*is_in_arg || !ft_ms_isspace(line[*i])))
		{
			*is_in_arg = quote_manager(line[*i], *is_in_arg);
			if (!(*is_in_arg) && ft_isredir(line[*i]))
				break ;
			(*i)++;
		}
	}
	return (word_count);
}

int	process_redir(char **split, char *line, int *i, int *k)
{
	if (line[*i + 1] == line[*i])
	{
		if (copy_arg(split, &line[*i], 2, (*k)++) == -1)
			return (1);
		(*i) += 2;
	}
	else
	{
		if (copy_arg(split, &line[*i], 1, (*k)++) == -1)
			return (1);
		(*i)++;
	}
	return (0);
}

int	ft_ms_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	quote_manager(char current, int is_in_arg)
{
	if (current == '\'' && is_in_arg != 2)
	{
		if (is_in_arg == 0)
			return (1);
		if (is_in_arg == 1)
			return (0);
	}
	else if (current == '"' && is_in_arg != 1)
	{
		if (is_in_arg == 0)
			return (2);
		if (is_in_arg == 2)
			return (0);
	}
	return (is_in_arg);
}
