/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 02:40:05 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 14:40:08 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirection	*add_redirection(t_redirection *head, t_token_type type,
		char *file)
{
	t_redirection	*new;
	t_redirection	*current;

	if (!file)
		return (head);
	new = malloc(sizeof(t_redirection));
	if (!new)
		return (ft_dprintf(2, ERR_MALLOC), NULL);
	new->type = type;
	new->file = ft_strdup(file);
	if (!new->file)
	{
		(free(file), free(new));
		return (ft_dprintf(2, ERR_MALLOC), NULL);
	}
	free(file);
	new->next = NULL;
	if (!head)
		return (new);
	current = head;
	while (current->next)
		current = current->next;
	current->next = new;
	return (head);
}

t_token	*remove_redirection_tokens(t_token *current, t_token **prev,
		t_command *command, bool remove_next)
{
	t_token	*to_free;

	if (remove_next && current->next)
	{
		to_free = current->next;
		if (*prev)
			(*prev)->next = current->next->next;
		else
			command->tokens = current->next->next;
		free_token(current);
		current = to_free->next;
		free_token(to_free);
	}
	else
	{
		if (*prev)
			(*prev)->next = current->next;
		else
			command->tokens = current->next;
		to_free = current->next;
		free_token(current);
		current = to_free;
	}
	return (current);
}

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

static bool	handle_heredoc(t_token *current, char **value,
		t_minishell *minishell)
{
	char	*delimiter;
	bool	need_expansion;
	char	*content;

	need_expansion = true;
	if (!current || !current->next || !current->next->value)
		return (false);
	delimiter = get_delimiter(current->next->value, &need_expansion);
	if (!delimiter)
		return (false);
	content = get_heredoc_content(delimiter);
	free(delimiter);
	if (!content)
		return (false);
	if (need_expansion)
	{
		*value = expand_line(content, minishell);
		free(content);
		return (*value != NULL);
	}
	*value = content;
	return (true);
}

static bool	handle_simple_redir(t_token *current, char **value,
		t_minishell *minishell)
{
	char	*tmp;

	tmp = ft_strdup(current->next->value);
	if (!tmp)
		return (ft_dprintf(2, ERR_MALLOC), false);
	*value = expand_line(tmp, minishell);
	return (*value != NULL);
}

static bool	process_redirection(t_token *current, t_redirection **redirections,
		t_minishell *minishell)
{
	char	*value;

	if (!current->next || current->next->type != REDIR_TARGET)
		return (false);
	if (current->type == REDIR_HEREDOC)
	{
		if (!handle_heredoc(current, &value, minishell))
			return (false);
	}
	else
	{
		if (!handle_simple_redir(current, &value, minishell))
			return (false);
	}
	*redirections = add_redirection(*redirections, current->type, value);
	return (true);
}

bool	parse_cmd_redirections(t_command *command, t_minishell *minishell)
{
	t_token			*prev;
	t_token			*current;
	t_redirection	*redirections;

	prev = NULL;
	current = command->tokens;
	redirections = NULL;
	while (current)
	{
		if (current->type >= REDIR_INPUT && current->type <= REDIR_HEREDOC)
		{
			if (!process_redirection(current, &redirections, minishell))
				return (free_redirections(redirections), false);
			current = remove_redirection_tokens(current, &prev, command, true);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	command->redirections = redirections;
	return (true);
}

bool	parse_redirections(t_command *commands, t_minishell *minishell)
{
	while (commands)
	{
		if (!parse_cmd_redirections(commands, minishell))
		{
			if (g_signal != 0)
			{
				minishell->status = g_signal;
				g_signal = 0;
			}
			if (minishell->status != 130)
			{
				minishell->status = 2;
				g_signal = 0;
				ft_dprintf(2, ERR_BAD_REDIRECTION);
			}
			return (false);
		}
		commands = commands->next;
	}
	return (true);
}
