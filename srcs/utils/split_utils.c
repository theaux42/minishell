/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 03:33:11 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/17 10:04:17 by tbabou           ###   ########.fr       */
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
	if (current == '\'' && is_in_arg != 2)
	{
		if (is_in_arg == 0)
			return (1);
		if (is_in_arg == 1)
			return (0);
	}
	else if (current == '"' && is_in_arg != 1)
	{
		if (is_in_arg == 0)
			return (2);
		if (is_in_arg == 2)
			return (0);
	}
	return (is_in_arg);
}
