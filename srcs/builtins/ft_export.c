/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/07 20:17:26 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_token *tokens, char ***env)
{
	char	**split;

	split = ft_split(tokens->value, '=');
	if (!split || !split[0] || !split[1])
	{
		fprintf(stderr, "Error: Invalid syntax\n");
		return (1);
	}
	set_env(split[0], split[1], env);
	ft_freesplit(split);
	return (0);
}
