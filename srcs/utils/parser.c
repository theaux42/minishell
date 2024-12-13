/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:40:40 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/13 17:22:28 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_redirection_type(char *str)
{
	if (strncmp(str, ">>", 2) == 0)
		return (REDIRECTION_APPEND);
	if (strncmp(str, ">", 1) == 0)
		return (REDIRECTION_OUTPUT);
	if (strncmp(str, "<", 1) == 0)
		return (REDIRECTION_INPUT);
	if (strncmp(str, "<<", 2) == 0)
		return (REDIRECTION_HEREDOC);
	return (ARGUMENT);
}

t_token_type	get_tokens_type(char *str, int pos)
{
	static t_token_type	last_type = COMMAND;
	t_token_type		redir_type;

	if (pos == 0)
		return (COMMAND);
	if (last_type == PIPE)
	{
		last_type = COMMAND;
		return (COMMAND);
	}
	if (strcmp(str, "|") == 0)
	{
		last_type = PIPE;
		return (PIPE);
	}
	redir_type = get_redirection_type(str);
	if (redir_type != ARGUMENT)
		return (redir_type);
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

int	handle_heredoc(char	*del)
{
	char	*line;
	int		fd[2];

	if (pipe(fd) == -1)
		perror("pipe");
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, del) == 0)
		{
			free(line);
			break;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}