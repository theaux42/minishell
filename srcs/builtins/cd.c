/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:20:31 by tbabou            #+#    #+#             */
/*   Updated: 2024/10/11 23:49:47 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char *line)
{
	char	*path;

	path = ft_strtrim(line, " ");
	if (chdir(path) == -1)
	{
		perror("cd");
		free(path);
		return (1);
	}
	free(path);
	return (1);
}
