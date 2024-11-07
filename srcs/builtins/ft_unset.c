/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 20:09:06 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 20:18:33 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_token *tokens, char ***env)
{
	char	*key;

	while (tokens)
	{
		key = ft_token_value(tokens->value);
		if (!key)
			return (perror("unset"), 1);
		del_env(key, env);
		free(key);
		tokens = tokens->next;
	}
	return (0);
}
