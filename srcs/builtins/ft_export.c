/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:10:48 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/14 06:05:29 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *expanded, char *line, int *i, char **env)
{
	int		start;
	int		len;
	char	*searched;
	char	*var;
	char	*tmp;

	start = *i;
	while (line[*i] && line[*i] != ' ')
		(*i)++;
	len = *i - start;
	searched = ft_substr(line, start, len);
	if (!searched)
		return (free(expanded), NULL);
	var = get_env(searched, env);
	if (!var)
		var = "";
	tmp = ft_strjoin(expanded, var);
	free(searched);
	if (!tmp)
		return (free(expanded), NULL);
	free(expanded);
	return (tmp);
}

char	*expand_line(char *line, char **env)
{
	int		i;
	char	*expanded;
	char	*tmp;

	i = 0;
	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			expanded = expand_var(expanded, line, &i, env);
		}
		else
		{
			tmp = ft_addchar(expanded, line[i]);
			expanded = tmp;
			i++;
		}
		if (!expanded)
			return (NULL);
	}
	return (expanded);
}

static char	*expand_quotes(char *value, char **env, char q)
{
	char	quote[2];
	char	*trim;
	char	*expanded;

	quote[1] = '\0';
	quote[0] = q;
	trim = ft_strtrim(value, quote);
	if (!trim)
		return (NULL);
	if (q == '"')
		expanded = expand_line(trim, env);
	else
		expanded = ft_strdup(trim);
	free(trim);
	return (expanded);
}

char	*expand_value(char *value, char **env)
{
	size_t	len;
	char	*expanded;

	len = ft_strlen(value);
	if (value[0] == '$')
	{
		expanded = get_env(value + 1, env);
		if (!expanded)
			return (ft_strdup(""));
		return (ft_strdup(expanded));
	}
	if (value[0] == '"' && value[len - 1] == '"')
		return (expand_quotes(value, env, '"'));
	if (value[0] == '\'' && value[len - 1] == '\'')
		return (expand_quotes(value, env, '\''));
	return (expand_line(value, env));
}

int	ft_export(t_token *tokens, char ***env)
{
	char	**split;
	char	*expanded_value;

	if (!tokens || !tokens->value)
		return (0);
	split = ft_split(tokens->value, '=');
	if (!split || !split[0] || !split[1])
	{
		ft_freesplit(split);
		fprintf(stderr, "export: Invalid syntax\n");
		return (1);
	}
	expanded_value = expand_value(split[1], *env);
	if (!expanded_value)
	{
		ft_freesplit(split);
		fprintf(stderr, "export: Memory error\n");
		return (1);
	}
	set_env(split[0], expanded_value, env);
	free(expanded_value);
	ft_freesplit(split);
	return (0);
}
