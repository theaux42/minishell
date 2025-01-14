/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 08:39:30 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/14 09:48:40 by tbabou           ###   ########.fr       */
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
