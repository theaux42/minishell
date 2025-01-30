/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2025/01/29 14:49:51 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static int	is_valid_export_format(char **split)
// {
// 	if (!split)
// 		return (0);
// 	if (!split[0] && !split[1])
// 		return (ft_freesplit(split), 0);
// 	if (*split[0] != '\0')
// 		return (1);
// 	return (1);
// }

static bool	is_only_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] == '=')
		i++;
	if (str[i] == '\0')
		return (true);
	return (false);
}

static char	*get_key(char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

static char	*get_value(char *str)
{
	int		i;
	char	*value;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (ft_strdup(""));
	value = ft_strdup(str + i + 1);
	return (value);
}

static int	handle_single_export(char *str, char ***env)
{
	char	*key;
	char	*value;
	int		has_equals;

	if (!str || str[0] == '=' || is_only_equal(str))
		return (ft_dprintf(2, ERR_EXPORT_INVALID, str), 1);
	has_equals = ft_strchr(str, '=') != NULL;
	key = get_key(str);
	if (!key)
		return (ft_dprintf(2, ERR_MALLOC), 1);
	if (!is_valid_key(key))
		return ((ft_dprintf(2, ERR_EXPORT_INVALID_ID, key), free(key)), 1);
	if (!has_equals)
		return ((set_env(key, "", env), free(key)), 0);
	value = get_value(str);
	if (!value)
		return ((ft_dprintf(2, ERR_MALLOC), free(key)), 1);
	set_env(key, value, env);
	free(key);
	free(value);
	return (0);
}

int	ft_export(t_token *tokens, char ***env)
{
	t_token	*cur;
	int		ret;
	int		token_count;

	token_count = 0;
	cur = tokens;
	ret = 0;
	if (!tokens && *env)
		return (ft_env(*env, true));
	while (cur)
	{
		token_count++;
		cur = cur->next;
	}
	cur = tokens;
	if (token_count == 1 && cur && (!cur->value || !*cur->value))
		return (ft_env(*env, true));
	while (cur)
	{
		if (handle_single_export(cur->value, env))
			ret = 1;
		cur = cur->next;
	}
	return (ret);
}
