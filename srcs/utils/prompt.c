/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 04:13:50 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 06:16:11 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_segment(char *segment, int index, bool add_slash)
{
	char	*result;
	char	short_name[2];

	if (!segment)
		return (NULL);
	if (index == 0 || index == 1 || !add_slash)
		result = ft_strdup(segment);
	else
	{
		short_name[0] = segment[0];
		short_name[1] = '\0';
		result = ft_strdup(short_name);
	}
	return (result);
}

char	*build_minimized_path(char **split)
{
	char	*new_pwd;
	char	*segment;
	bool	add_slash;
	int		i;

	if (!split || !split[0])
		return (ft_strdup("/"));
	new_pwd = ft_strdup("");
	if (!new_pwd)
		return (NULL);
	i = 0;
	while (split[i])
	{
		add_slash = (split[i + 1] != NULL);
		segment = get_path_segment(split[i], i, add_slash);
		if (!segment)
			return (free(new_pwd), NULL);
		new_pwd = append_path_segment(new_pwd, segment, add_slash);
		free(segment);
		if (!new_pwd)
			return (NULL);
		i++;
	}
	return (new_pwd);
}

char	*nice_pwd(char **env)
{
	char	*pwd;
	char	*home;
	char	**split;
	char	*tilde_replaced;
	char	*new_pwd;

	pwd = get_env("PWD", env);
	home = get_env("HOME", env);
	if (!pwd)
		return (NULL);
	tilde_replaced = replace_home_with_tilde(pwd, home);
	if (!tilde_replaced)
		return (NULL);
	split = ft_split(tilde_replaced, '/');
	free(tilde_replaced);
	if (!split)
		return (NULL);
	new_pwd = build_minimized_path(split);
	ft_freesplit(split);
	return (new_pwd);
}

char	*build_prompt(char **env, char *pwd)
{
	char	*final_prompt;
	char	*prompt[8];
	char	*user;

	user = get_env("USER", env);
	if (!user)
	{
		user = "user";
	}
	prompt[0] = AMBER500;
	prompt[1] = user;
	prompt[2] = RESET;
	prompt[3] = "@";
	prompt[4] = VIOLET500;
	prompt[5] = pwd;
	prompt[6] = RESET;
	prompt[7] = " → ";
	final_prompt = ft_strjoins(prompt, 8);
	return (final_prompt);
}

char	*nice_prompt(char **env)
{
	char	*new_pwd;
	char	*final_prompt;

	new_pwd = nice_pwd(env);
	if (!new_pwd)
		return (ft_strdup("Error building prompt"));
	final_prompt = build_prompt(env, new_pwd);
	free(new_pwd);
	if (!final_prompt)
		return (ft_strdup("Error building prompt"));
	return (final_prompt);
}
