/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/27 16:48:24 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_export_format(char **split)
{
	if (!split)
		return (0);
	if (!split[0] && !split[1])
		return (ft_freesplit(split), 0);
	if (*split[0] != '\0')
		return (1);
	return (1);
}

static bool is_only_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '=')
		i++;
	if (str[i] == '\0')
		return (true);
	return (false);
}

static int	handle_single_export(char *str, char ***env)
{
    char	**split;

    if (!str || str[0] == '=' || is_only_equal(str))
        return (ft_dprintf(2, ERR_EXPORT_INVALID, str), 1);
    split = ft_split(str, '=');
    if (!split)
        return (ft_dprintf(2, ERR_MALLOC), 1);
    if (!is_valid_export_format(split))
    {
        ft_freesplit(split);
        return (ft_dprintf(2, ERR_EXPORT_INVALID, str), 1);
    }
    if (!is_valid_key(split[0]))
    {
        ft_dprintf(2, ERR_EXPORT_INVALID_ID, split[0]);
        return (ft_freesplit(split), 1);
    }
    if (ft_split_len(split) == 1)
        set_env(split[0], "", env);
    else
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
	if (!tokens && *env)
		return (ft_env(*env, true));
	if (!cur->value || !*cur->value)
		return (ft_env(*env, true));
	while (cur)
	{
		if (handle_single_export(cur->value, env))
			ret = 1;
		cur = cur->next;
	}
	return (ret);
}
