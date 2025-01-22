/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:40:40 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/22 10:22:30 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_redirection_type(char *str)
{
	if (strncmp(str, ">>", 2) == 0)
		return (REDIR_APPEND);
	if (strncmp(str, ">", 1) == 0)
		return (REDIR_OUTPUT);
	if (strncmp(str, "<<", 2) == 0)
		return (REDIR_HEREDOC);
	if (strncmp(str, "<", 1) == 0)
		return (REDIR_INPUT);
	return (ARGUMENT);
}

t_token_type	get_tokens_type(char *str, int pos)
{
	static t_token_type	last_type = COMMAND;
	t_token_type		redir_type;

	redir_type = get_redirection_type(str);
	if (redir_type != ARGUMENT)
	{
		last_type = redir_type;
		return (redir_type);
	}
	if (pos == 0 || last_type == PIPE)
	{
		last_type = COMMAND;
		return (COMMAND);
	}
	last_type = ARGUMENT;
	return (ARGUMENT);
}

char	*ft_token_value(char *value)
{
	if (ft_edgecmp(value, '"'))
		return (ft_strtrim(value, "\""));
	if (ft_edgecmp(value, '\''))
		return (ft_strtrim(value, "'"));
	return (ft_strdup(value));
}
