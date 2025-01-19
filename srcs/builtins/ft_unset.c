/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:09:06 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/19 17:26:18 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_token *tokens, char ***env)
{
	if (!tokens || !tokens->value)
		return (0);
	while (tokens)
	{
		if (tokens->value)
			del_env(tokens->value, env);
		tokens = tokens->next;
	}
	return (0);
}
