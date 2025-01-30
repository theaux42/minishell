/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:52:59 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 06:34:53 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_prompt_length(char *line)
{
	int	i;
	int	word_count;
	int	is_in_arg;

	i = 0;
	word_count = 0;
	is_in_arg = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		while (line[i] && ft_ms_isspace(line[i]))
			i++;
		if (line[i])
			word_count += handle_token(line, &i, &is_in_arg);
	}
	return (word_count);
}

int	copy_arg(char **split, char *line, int j, int k)
{
	if (!split || !line || j < 0 || k < 0)
		return (-1);
	split[k] = malloc(sizeof(char) * (j + 1));
	// if (k == 2)
	// {
	// 	free(split[k]);
	// 	split[k] = NULL;
	// }
	if (!split[k])
	{
		while (--k >= 0)
			free(split[k]);
		free(split);
		return (ft_dprintf(2, ERR_MALLOC), 1);
	}
	ft_strncpy(split[k], line, j);
	split[k][j] = '\0';
	return (0);
}

int	process_argument(char **split, char *line, int *i, int *k)
{
	int	start;
	int	is_in_arg;

	start = *i;
	is_in_arg = 0;
	while (line[*i] && (is_in_arg || !ft_ms_isspace(line[*i])))
	{
		is_in_arg = quote_manager(line[*i], is_in_arg);
		if (!is_in_arg && ft_isredir(line[*i]))
		{
			if (*i > start)
				if (copy_arg(split, &line[start], *i - start, (*k)++))
					return (-1);
			if (process_redir(split, line, i, k))
				return (-1);
			start = *i;
		}
		else
			(*i)++;
	}
	if (*i > start)
		if (copy_arg(split, &line[start], *i - start, (*k)++))
			return (-1);
	return (is_in_arg);
}

int	inner_split(char **split, char *line)
{
	int	i;
	int	k;
	int	is_in_arg;

	i = 0;
	k = 0;
	is_in_arg = 0;
	if (check_missused_quotes(line))
		return (1);
	while (line[i])
	{
		while (line[i] && ft_ms_isspace(line[i]))
			i++;
		if (line[i] && !ft_ms_isspace(line[i]))
		{
			is_in_arg = process_argument(split, line, &i, &k);
			if (is_in_arg == -1)
				return (-1);
		}
	}
	split[k] = NULL;
	return (is_in_arg);
}

char	**ft_ms_split(char *line)
{
	char	**args;
	int		result;
	int		prompt_length;

	if (!line)
		return (NULL);
	prompt_length = ft_prompt_length(line) + 1;
	if (prompt_length <= 0)
		return (NULL);
	args = malloc(sizeof(char *) * (prompt_length + 1));
	if (!args)
		return (NULL);
	ft_memset(args, 0, sizeof(char *) * (prompt_length + 1));
	result = inner_split(args, line);
	if (result == -1)
		return (NULL);
	if (result == 1 || result == 2)
	{
		ft_freesplit(args);
		ft_dprintf(2, ERR_UNCLOSED_QUOTES);
		return (NULL);
	}
	return (args);
}
