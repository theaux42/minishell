/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 08:39:30 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/21 12:19:35 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	validate_commands(t_command *commands)
{
	t_command	*cur;
	t_token		*tok;

	cur = commands;
	while (cur)
	{
		tok = cur->tokens;
		while (tok && tok->type != COMMAND && tok->type != ARGUMENT)
			tok = tok->next;
		if (!tok)
			return (ft_dprintf(2, ERR_EMPTY_CMD), false);
		cur = cur->next;
	}
	return (true);
}

void	error_message(char *title, char *message)
{
	perror(title);
	ft_dprintf(2, "%s\n", message);
}

void	exit_parent(char *msg, t_minishell *minishell)
{
	if (msg != NULL)
		ft_dprintf(2, "%s", msg);
	if (minishell->commands)
		free_commands(minishell->commands);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	exit(EXIT_FAILURE);
}

void	exit_child(char *msg, t_minishell *minishell, char *cmd, char **argv)
{
	if (msg != NULL)
		ft_dprintf(2, "%s", msg);
	free_commands(minishell->commands);
	rl_clear_history();
	ft_freesplit(minishell->env);
	free(minishell->line);
	free(minishell);
	free(cmd);
	free(argv);
	exit(EXIT_FAILURE);
}
