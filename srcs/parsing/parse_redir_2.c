/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 21:18:03 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 21:38:40 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_content(char *delimiter)
{
	char	*line;
	char	*result;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (1)
	{
		line = readline(HEREDOC_PROMPT);
		if (g_signal != 0)
			return (free(result), NULL);
		if (!line)
			return (result);
		if (ft_strcmp(line, delimiter) == 0)
			return (free(line), result);
		result = ft_strjoin_double(result, line, "\n", 2);
		if (!result)
			return (free(line), NULL);
	}
}

static char	*get_delimiter(char *delimiter, bool *need_expansion)
{
	char	*tmp;

	tmp = ft_strdup(delimiter);
	if (!tmp)
		return (NULL);
	delimiter = process_quote(tmp);
	if (!delimiter)
		return ((free(tmp)), NULL);
	if (ft_strcmp(tmp, delimiter) == 0)
		*need_expansion = true;
	else
		*need_expansion = false;
	free(tmp);
	return (delimiter);
}

bool	handle_heredoc_parsing(t_token *current, char **value,
		t_minishell *minishell)
{
	char	*delimiter;
	bool	need_expansion;

	need_expansion = true;
	if (!current || !current->next || !current->next->value)
		return (false);
	delimiter = get_delimiter(current->next->value, &need_expansion);
	if (!delimiter)
		return (false);
	*value = get_heredoc_content(delimiter);
	free(delimiter);
	if (!*value)
		return (false);
	if (need_expansion)
		*value = expand_line(*value, minishell);
	return (true);
}

bool	handle_simple_redir(t_token *current, char **value,
		t_minishell *minishell)
{
	char	*tmp;

	tmp = ft_strdup(current->next->value);
	if (!tmp)
		return (ft_dprintf(2, ERR_MALLOC), false);
	*value = expand_line(tmp, minishell);
	return (*value != NULL);
}
