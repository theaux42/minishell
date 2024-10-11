/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:52:59 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 23:49:47 by tbabou           ###   ########.fr       */
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
	while (line[i])
	{
		while (line[i] && ft_ms_isspace(line[i]))
			i++;
		if (line[i])
		{
			word_count++;
			while (line[i] && (is_in_arg != 0 || !ft_ms_isspace(line[i])))
			{
				is_in_arg = quote_manager(line[i], is_in_arg);
				i++;
			}
		}
	}
	return (word_count);
}

int	copy_arg(char **split, char *line, int j, int k)
{
	split[k] = malloc(j + 1);
	if (!split[k])
		return (-1);
	ft_strncpy(split[k], line, j);
	split[k][j] = '\0';
	return (0);
}

int	process_argument(char **split, char *line, int *i, int *k)
{
	int	j;
	int	is_in_arg;

	j = *i;
	is_in_arg = 0;
	while (line[*i] && (is_in_arg != 0 || !ft_ms_isspace(line[*i])))
		is_in_arg = quote_manager(line[(*i)++], is_in_arg);
	if (copy_arg(split, &line[j], *i - j, *k) == -1)
		return (-1);
	(*k)++;
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
	return (is_in_arg);
}

char	**ft_ms_split(char *line)
{
	char	**args;
	int		result;

	args = malloc(sizeof(char *) * (ft_prompt_length(line) + 1));
	if (!args)
		return (NULL);
	result = inner_split(args, line);
	if (result == -1)
		return (ft_freesplit(args), NULL);
	if (result != 0)
	{
		ft_freesplit(args);
		printf("%sInvalid prompt%s\n", RED800, RESET);
		return (NULL);
	}
	args[ft_prompt_length(line)] = NULL;
	return (args);
}
