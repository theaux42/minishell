/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:15:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 06:15:02 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_token *tokens)
{
	t_token	*cur;
	int		newline;

	cur = tokens;
	newline = 0;
	if (cur && ft_strncmp(cur->value, "-n", ft_strlen(cur->value)) == 0)
		cur = cur->next;
	else
		newline = 1;
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
