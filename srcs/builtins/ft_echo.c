/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:15:00 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/21 12:16:12 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	good_flag(const char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || !str[1])
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_token *tokens)
{
	t_token	*cur;
	int		newline;

	cur = tokens;
	newline = 1;
	while (cur && good_flag(cur->value))
	{
		newline = 0;
		cur = cur->next;
	}
	while (cur)
	{
		printf("%s", cur->value);
		cur = cur->next;
		if (cur)
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}
