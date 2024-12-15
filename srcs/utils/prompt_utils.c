/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 04:15:56 by tbabou            #+#    #+#             */
/*   Updated: 2024/12/15 04:17:45 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_home_with_tilde(char *pwd, char *home)
{
	if (home && ft_strncmp(pwd, home, ft_strlen(home)) == 0)
		return (ft_strjoin("~", pwd + ft_strlen(home)));
	return (ft_strdup(pwd));
}

char	*append_path_segment(char *current_path, char *segment, bool add_slash)
{
	char	*temp;
	char	*result;

	if (!current_path || !segment)
		return (NULL);
	temp = ft_strjoin(current_path, segment);
	free(current_path);
	if (!temp)
		return (NULL);
	if (add_slash)
	{
		result = ft_strjoin(temp, "/");
		free(temp);
		return (result);
	}
	return (temp);
}
