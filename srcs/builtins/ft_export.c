/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/19 13:21:21 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_export_format(char *str)
{
	char	**split;

	split = ft_split(str, '=');
	if (!split)
		return (0);
	if (!split[0] || !split[1])
		return (ft_freesplit(split), 0);
	ft_freesplit(split);
	return (1);
}

static int	handle_single_export(char *str, char ***env)
{
	char	**split;
	char	*processed_value;

	if (!is_valid_export_format(str))
		return (ft_dprintf(2, ERR_EXPORT_INVALID, str), 1);
	split = ft_split(str, '=');
	if (!split)
		return (ft_dprintf(2, ERR_MALLOC), 1);
	if (!is_valid_key(split[0]))
	{
		ft_dprintf(2, ERR_EXPORT_INVALID_ID, split[0]);
		return (ft_freesplit(split), 1);
	}
	processed_value = process_quote(split[1]);
	if (!processed_value)
	{
		ft_freesplit(split);
		return (ft_dprintf(2, ERR_MALLOC), 1);
	}
	free(split[1]);
	split[1] = processed_value;
	set_env(split[0], split[1], env);
	ft_freesplit(split);
	return (0);
}

int	ft_export(t_token *tokens, char ***env)
{
	t_token	*cur;
	int		ret;

	cur = tokens;
	ret = 0;
	while (cur)
	{
		if (handle_single_export(cur->value, env))
			ret = 1;
		cur = cur->next;
	}
	return (ret);
}
