/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 16:15:00 by tbabou            #+#    #+#             */
/*   Updated: 2024/11/25 09:58:26 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_env(char *searched, char c, char **env)
{
	char	*var;

	var = get_env(searched, env);
	if (var)
		printf("%s%c", var, c);
	else
		printf("%c", c);
}
	
int	parse_env(char *line, char **env)
{
	int		i;
	int		start;
	char	*searched;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			start = i;
			while (line[i] && line[i] != ' ' && line[i] != '\0')
				i++;
			searched = ft_mstrlcpy(&line[start], i - start);
			if (!searched)
				return (1);
			display_env(searched, line[i], env);
			free(searched);
		}
		else
			printf("%c", line[i]);
		i++;
	}
	return (0);
}

void	display_current(char *value, char **env)
{
	char	*str;

	if (value[0] == '$')
	{
		str = get_env(value + 1, env);
		if (str)
		{
			printf("%s", str);
		}
		else
			printf("%s", "");
	}
	else if (value[0] == '"' || value[ft_strlen(value) - 1] == '"')
		parse_env(ft_strtrim(value, "\""), env);
	else if (value[0] == '\'' || value[ft_strlen(value) - 1] == '\'')
		printf("%s", ft_strtrim(value, "'"));
	else
		printf("%s", value);
}

int	ft_echo(t_token *tokens, char **env)
{
	t_token	*cur;
	int		newline;

	cur = tokens;
	newline = 0;
	if (cur && ft_strncmp(cur->value, "-n", ft_strlen(cur->value)) == 0)
		cur = cur->next;
	else
		newline = 1;
	while (cur)
	{
		display_current(cur->value, env);
		cur = cur->next;
		if (cur)
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}
