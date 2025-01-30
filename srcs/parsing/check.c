/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 06:21:26 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/30 06:35:10 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_empty_pipes(char **tokens)
{
	bool	last_is_pipe;
	int		i;

	if (!tokens)
		return (false);
	i = 0;
	last_is_pipe = false;
	while (tokens[i])
	{
		if (ft_safecmp(tokens[i], "|") == 0)
		{
			if (last_is_pipe)
				return (ft_dprintf(2, ERR_EMPTY_CMD), false);
			last_is_pipe = true;
		}
		else
			last_is_pipe = false;
		i++;
	}
	return (true);
}
