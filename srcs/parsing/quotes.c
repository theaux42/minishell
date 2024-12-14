/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 03:27:47 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 06:01:13 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_missused_quotes(const char *str)
{
	int	in_s;
	int	in_d;
	int	i;

	if (!str)
		return (0);
	in_s = 0;
	in_d = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_s = !in_s;
		else if (str[i] == '"')
			in_d = !in_d;
		i++;
	}
	return (in_s || in_d);
}

size_t	ft_strlen_quote(const char *str, char quote)
{
	int		i;
	size_t	len;

	i = 0;
	len = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != quote)
			len++;
		i++;
	}
	return (len);
}

char	define_quote(char *line)
{
	if (line[0] == '\'' && line[ft_strlen(line) - 1] == '\'')
		return ('\'');
	else if (line[0] == '"' && line[ft_strlen(line) - 1] == '"')
		return ('"');
	return ('\0');
}

char	*process_quote(char *line)
{
	char	quote_type;
	char	*new_line;
	char	*new_line_ptr;
	char	*line_ptr;

	quote_type = define_quote(line);
	if (quote_type == '\0')
		return (line);
	new_line = ft_calloc(ft_strlen_quote(line, quote_type) + 1, sizeof(char));
	if (!new_line)
		return (NULL);
	new_line_ptr = new_line;
	line_ptr = line;
	while (*line++)
	{
		if (*line != quote_type)
		{
			*new_line_ptr = *line;
			new_line_ptr++;
		}
	}
	*new_line_ptr = '\0';
	free(line_ptr);
	return (new_line);
}
