/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/08 07:48:34 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_export_format(char *str)
{
	char	**split;

	split = ft_split(str, '=');
	if (!split || !split[0] || !split[1])
	{
		ft_freesplit(split);
		return (0);
	}
	ft_freesplit(split);
	return (1);
}

static int	handle_single_export(char *str, char ***env)
{
	char	**split;

	if (!is_valid_export_format(str))
	{
		printf("export: Invalid syntax: '%s'\n", str);
		return (1);
	}
	split = ft_split(str, '=');
	if (!is_valid_key(split[0]))
	{
		printf("export: '%s': not a valid identifier\n", split[0]);
		ft_freesplit(split);
		return (1);
	}
	split[1] = process_quote(split[1]);
	set_env(split[0], split[1], env);
	ft_freesplit(split);
	return (0);
}

int	ft_export(t_token *tokens, char ***env)
{
	t_token	*cur;
	int		ret;
	int		error;

	cur = tokens;
	ret = 0;
	while (cur)
	{
		error = handle_single_export(cur->value, env);
		if (error)
			ret = 1;
		cur = cur->next;
	}
	return (ret);
}
