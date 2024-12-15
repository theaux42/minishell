/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ededemog <ededemog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 17:37:04 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 14:35:09 by ededemog         ###   ########.fr       */
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

	start = *line;
	while (**line && (in_quotes || !ft_ms_isspace(**line)))
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

	// Advance past any additional whitespace
	while (ft_ms_isspace(**line))
		(*line)++;

	return (token);
}

t_redirection	*get_redirection(char *line)
{
	t_redirection	*head;
	t_redirection	*current;
	t_redirection	*new_redir;
	char			*token;
	int				redir_type;

	head = NULL;
	current = NULL;
	while ((token = get_next_token(&line)))
	{
		redir_type = get_redirection_type(token);
		if (redir_type == ARGUMENT)
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
		new_redir->type = redir_type;
		new_redir->next = NULL;
		new_redir->fd = -1;

		// Get the next token as the filename or delimiter
		char *filename = get_next_token(&line);
		if (!filename)
		{
			fprintf(stderr, "minishell: syntax error: redirection requires a filename\n");
			free(token);
			free(new_redir);
			free_redirection(head);
			return (NULL);
		}
		new_redir->file = filename;

		free(token);
		if (!head)
			head = new_redir;
		else
			current->next = new_redir;
		current = new_redir;
	}
	return head;
}

int handle_heredoc_redirection(t_redirection *redir)
{
    int heredoc_fd;

    heredoc_fd = handle_heredoc(redir->file);
    if (heredoc_fd == -1)
        return -1;

    redir->fd = heredoc_fd;
    return 0;
}