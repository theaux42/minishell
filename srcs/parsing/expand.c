/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:25:30 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 05:40:56 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(char *line, int *i, char *new_line,
		t_minishell *minishell)
{
	char	*current_key;
	char	*value;
	char	*temp;

	(*i)++;
	current_key = get_current_key(line, i);
	if (!current_key)
	{
		new_line = ft_addchar(new_line, '$');
		return (new_line);
	}
	value = get_env_var(current_key, minishell);
	free(current_key);
	if (!value)
		return (NULL);
	temp = ft_strjoin(new_line, value);
	free(new_line);
	free(value);
	if (!temp)
		return (NULL);
	return (temp);
}

char	*expand_line(char *line, t_minishell *minishell)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = ft_calloc(1, 1);
	if (!new_line)
		return (NULL);
	while (line[i] && line[i] != '\0')
	{
		if (line[i] == '$')
		{
			new_line = expand_env_var(line, &i, new_line, minishell);
			if (!new_line)
				return (NULL);
		}
		else
		{
			new_line = ft_addchar(new_line, line[i++]);
			if (!new_line)
				return (NULL);
		}
	}
	free(line);
	return (new_line);
}

static t_token	*create_arg_token(char *value, t_token *next)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = ARGUMENT;
	new->next = next;
	return (new);
}

static int	split_expanded_token(t_token *token, char **split)
{
	t_token	*current;
	int		i;

	if (!split[0] || !split[1])
		return (0);
	free(token->value);
	token->value = ft_strdup(split[0]);
	if (!token->value)
		return (-1);
	current = token;
	i = 1;
	while (split[i])
	{
		current->next = create_arg_token(split[i], current->next);
		if (!current->next)
			return (-1);
		current = current->next;
		i++;
	}
	return (0);
}

void	process_token(t_token *token, t_minishell *minishell)
{
	char	*temp;
	char	**split;

	if (need_expansion(token->value))
	{
		token->value = expand_line(token->value, minishell);
		if (!token->value)
			exit_parent(ERR_MALLOC, minishell, true);
		if ((token->type == COMMAND || token->type == ARGUMENT) && token->value
			&& token->value[0] != '\0')
		{
			split = ft_split(token->value, ' ');
			if (!split)
				exit_parent(ERR_MALLOC, minishell, true);
			if (split_expanded_token(token, split) == -1)
				exit_parent(ERR_MALLOC, minishell, true);
			ft_freesplit(split);
		}
	}
	temp = token->value;
	token->value = process_quote(token->value);
	if (!token->value)
		exit_parent(ERR_MALLOC, minishell, true);
	free(temp);
}

bool	process_commands(t_token *tokens, t_minishell *minishell)
{
	char	*cmd;

	cmd = get_full_cmd(tokens->value, minishell->env);
	if (!cmd)
	{
		minishell->status = 127;
		ft_dprintf(2, ERR_CMD_NOT_FOUND, tokens->value);
		tokens->type = ARGUMENT;
		if (minishell->cmd_count > 1)
			return (true);
		return (false);
	}
	if ((access(cmd, F_OK | X_OK) != 0 && !is_builtin(cmd)) || ft_isfolder(cmd))
	{
		minishell->status = 126;
		if (ft_isfolder(cmd))
			ft_dprintf(2, ERR_IS_FOLDER, tokens->value);
		else if (access(cmd, F_OK | X_OK) != 0 && !is_builtin(cmd))
			ft_dprintf(2, ERR_NO_RIGHT, tokens->value);
		return (free(cmd), false);
	}
	free(tokens->value);
	tokens->value = cmd;
	tokens->type = COMMAND;
	return (true);
}

static bool	expand_tokens(t_command *command, t_minishell *minishell)
{
	t_token	*current_token;
	int		pos;

	pos = 0;
	current_token = command->tokens;
	while (current_token)
	{
		process_token(current_token, minishell);
		if (current_token->type == COMMAND || pos == 0)
		{
			current_token->type = ARGUMENT;
			if (!process_commands(current_token, minishell))
				if (!command->redirections)
					return (false);
		}
		current_token = current_token->next;
		pos++;
	}
	return (true);
}

bool	expand_commands(t_command *commands, t_minishell *minishell)
{
	t_command	*current;
	int			i;

	i = 0;
	current = commands;
	if (!check_commands(commands))
		return (ft_dprintf(2, ERR_UNCLOSED_QUOTES), false);
	while (current)
	{
		if (!expand_tokens(current, minishell))
			return (false);
		current = current->next;
		i++;
	}
	return (true);
}
