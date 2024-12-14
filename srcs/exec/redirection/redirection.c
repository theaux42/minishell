/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 17:26:07 by ededemog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Redirections:
// Since they are attached to the command, we create a new structure for them.
// This structure will contain the type of the redirection, the target file
// and a pointer to the next redirection.
// The parsing of it will be done in the get_redirections function.
// The function will create the structure and attach it to the command.
// and remove the redirection from the tokens list.
// Need to check if the redirection is well formed.
// There can't be two file redirections in a row.

static char	*ft_strndup(const char *s, size_t n)
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1));
	if (res == NULL)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

static void	free_redirection(t_redirection *redirection)
{
	t_redirection	*tmp;

	while (redirection)
	{
		tmp = redirection;
		redirection = redirection->next;
		free(tmp->file);
		if (tmp->fd != 0)
			close(tmp->fd);
		free(tmp);
	}
}

char	*get_next_token(char **line)
{
	char	*start;
	char	*token;
	int		in_quotes = 0;

	while (ft_ms_isspace(**line))
		(*line)++;
	if (**line == '\0')
		return (NULL);
	printf("line: %s\n", *line);
	start = *line;
	while(**line && (in_quotes || !ft_ms_isspace(**line)))
	{
		if (**line == '\'' || **line == '"')
		{
			in_quotes = quote_manager(**line, in_quotes);
			(*line)++;
			continue;
		}
		(*line)++;
	}
	token = ft_strndup(start, *line - start);
	while (ft_ms_isspace(**line))
		(*line)++;
	return (token);
}

t_redirection	*get_redirection(t_command *command, char *line)
{
	t_redirection	*head;
	t_redirection	*current;
	t_redirection	*new_redir;
	char			*delimeter;
	char			*token;

	head = NULL;
	current = NULL;
	(void)command;
	while ((token = get_next_token(&line)))
	{
		if (get_redirection_type(token) == ARGUMENT)
		{
			free(token);
			break;
		}
		new_redir = malloc(sizeof(t_redirection));
		if (!new_redir)
		{
			free(token);
			free_redirection(head);
			return (NULL);
		}
		new_redir->next = NULL;
		new_redir->type = get_redirection_type(token);
		new_redir->fd = -1;
		if (new_redir->type == REDIRECTION_HEREDOC)
		{
			delimeter = get_next_token(&line);
			printf("delimeter: %s\n", delimeter);
			if (!delimeter || *delimeter == '\0')
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
				free(new_redir);
				free_redirection(head);
				return (NULL);
			}
			new_redir->file = ft_strdup(delimeter);
			new_redir->fd = handle_heredoc(delimeter);
			if (new_redir->fd == -1)
			{
				free(new_redir->file);
				free(new_redir);
				free_redirection(head);
				return (NULL);
			}
			free(delimeter);
		}
		else
		{
			new_redir->file = get_next_token(&line);
			new_redir->fd = -1;
		}
		free(token);
		if (!head)
			head = new_redir;
		else
			current->next = new_redir;
		current = new_redir;
	}
	return (head);
}