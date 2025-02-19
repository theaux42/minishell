/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:09:06 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 13:56:43 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_token *tokens, char ***env)
{
	int	ret;

	ret = 0;
	if (!tokens || !tokens->value)
		return (0);
	while (tokens)
	{
		if (tokens->value)
		{
			if (is_valid_key(tokens->value))
				del_env(tokens->value, env);
			else
			{
				ft_dprintf(2, ERR_UNSET_INVALID_ID, tokens->value);
				ret = 1;
			}
		}
		tokens = tokens->next;
	}
	return (ret);
}
