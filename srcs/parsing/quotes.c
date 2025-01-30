/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 03:27:47 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 15:54:30 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_missused_quotes(const char *str)
{
    int	in_s;
    int	in_d;
    int	i;

    if (!str)
        return (-1);
    if (!*str)
        return (-1);
    in_s = 0;
    in_d = 0;
    i = 0;
    while (str[i])
    {
        if (str[i] == '\'' && !in_d)
            in_s = !in_s;
        else if (str[i] == '"' && !in_s)
            in_d = !in_d;
        i++;
    }
    return (in_s || in_d);
}

bool	handle_quotes(char c, bool *in_single_quote, bool *in_double_quote)
{
	if (c == '\'' && !*in_double_quote)
	{
		*in_single_quote = !*in_single_quote;
		return (true);
	}
	else if (c == '"' && !*in_single_quote)
	{
		*in_double_quote = !*in_double_quote;
		return (true);
	}
	return (false);
}

char	*build_new_line(char *line)
{
	char	*new_line;
	int		i;
	int		j;
	bool	in_single_quote;
	bool	in_double_quote;

	new_line = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	if (!new_line)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (line[i])
	{
		if (handle_quotes(line[i], &in_single_quote, &in_double_quote))
			;
		else
			new_line[j++] = line[i];
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*process_quote(char *line)
{
	char	*new_line;

	if (!line)
		return (NULL);
	new_line = build_new_line(line);
	if (!new_line)
		return (NULL);
	return (new_line);
}
