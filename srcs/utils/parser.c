/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:40:40 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 14:53:13 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_redirection_type(char *str)
{
	if (strncmp(str, ">>", 2) == 0)
		return (REDIRECTION_APPEND);
	if (strncmp(str, ">", 1) == 0)
		return (REDIRECTION_OUTPUT);
	if (strncmp(str, "<<", 2) == 0)
		return (REDIRECTION_HEREDOC);
	if (strncmp(str, "<", 1) == 0)
		return (REDIRECTION_INPUT);
	return (ARGUMENT);
}

t_token_type	get_tokens_type(char *str, int pos)
{
	static t_token_type	last_type = COMMAND;
	t_token_type		redir_type;

	if (pos == 0)
	{
		last_type = COMMAND;
		return (COMMAND);
	}

	if (strcmp(str, "|") == 0)
	{
		last_type = PIPE;
		return (PIPE);
	}
	
	if (last_type == PIPE)
	{
		last_type = COMMAND;
		return (COMMAND);
	}

	redir_type = get_redirection_type(str);
	if (redir_type != ARGUMENT)
	{
		last_type = redir_type;
		return (redir_type);
	}

	if (last_type == REDIRECTION_HEREDOC || 
		last_type == REDIRECTION_INPUT ||
		last_type == REDIRECTION_OUTPUT ||
		last_type == REDIRECTION_APPEND)
	{
		t_token_type prev_type = last_type;
		last_type = ARGUMENT;
		return (prev_type == REDIRECTION_HEREDOC ? HEREDOC_DELIMITER : ARGUMENT);
	}

	last_type = ARGUMENT;
	return (ARGUMENT);
}

char	*ft_token_value(char *value)
{
	int	len;

	len = ft_strlen(value);
	if (value[0] == '"' && value[len - 1] == '"')
		return (ft_strtrim(value, "\""));
	if (value[0] == '\'' && value[len - 1] == '\'')
		return (ft_strtrim(value, "'"));
	return (ft_strdup(value));
}

int handle_heredoc(const char *delimiter)
{
    int fd[2];
    char *line;

    if (pipe(fd) == -1)
        return -1;

	setup_heredoc_signals();
    while (1)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
	restore_signals();
    close(fd[1]);
    return fd[0];
}