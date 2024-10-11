/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:33:11 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 23:49:47 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_ms_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	quote_manager(char current, int is_in_arg)
{
	if (current == '\'')
	{
		if (is_in_arg == 0)
			is_in_arg = 1;
		else if (is_in_arg == 1)
			is_in_arg = 0;
	}
	else if (current == '"')
	{
		if (is_in_arg == 0)
			is_in_arg = 2;
		else if (is_in_arg == 2)
			is_in_arg = 0;
	}
	return (is_in_arg);
}
