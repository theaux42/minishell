/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 02:40:40 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/31 23:54:22 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	get_redirection_type(char *str)
{
	if (ft_strncmp(str, ">>", 2) == 0)
		return (REDIR_APPEND);
	if (ft_strncmp(str, ">", 1) == 0)
		return (REDIR_OUTPUT);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (REDIR_HEREDOC);
	if (ft_strncmp(str, "<", 1) == 0)
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
	if (last_type == REDIR_HEREDOC || last_type == REDIR_OUTPUT
		|| last_type == REDIR_APPEND || last_type == REDIR_INPUT)
	{
		last_type = REDIR_TARGET;
		return (REDIR_TARGET);
	}
	last_type = ARGUMENT;
	return (ARGUMENT);
}

t_command	*init_new_command(void)
{
	t_command	*new_command;

	new_command = malloc(sizeof(t_command));
	if (!new_command)
		return (NULL);
	new_command->tokens = NULL;
	new_command->redirections = NULL;
	new_command->pipes[0] = -1;
	new_command->pipes[1] = -1;
	new_command->prev_pipe = -1;
	new_command->is_absolute = 0;
	new_command->is_last = 0;
	new_command->pid = 0;
	new_command->next = NULL;
	return (new_command);
}

// static t_token	*init_new_token(void)
// {
// 	t_token	*new_token;

// 	new_token = malloc(sizeof(t_token));
// 	if (!new_token)
// 		return (NULL);
// 	new_token->value = NULL;
// 	new_token->type = COMMAND;
// 	new_token->next = NULL;
// 	return (new_token);
// }
