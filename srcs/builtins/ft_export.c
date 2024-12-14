/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 07:30:41 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_export(t_token *tokens, char ***env)
{
	char	**split;

	if (!tokens || !tokens->value)
		return (0);
	split = ft_split(tokens->value, '=');
	if (!split || !split[0] || !split[1])
	{
		fprintf(stderr, "export: Invalid syntax\n");
		ft_freesplit(split);
		return (1);
	}
	split[1] = process_quote(split[1]);
	if (is_valid_key(split[0]))
		set_env(split[0], split[1], env);
	else
	{
		fprintf(stderr, "export: '%s': not a valid identifier\n", split[0]);
		ft_freesplit(split);
		return (1);
	}
	ft_freesplit(split);
	return (0);
}
