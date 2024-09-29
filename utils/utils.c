/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbabou <tbabou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:53:44 by tbabou            #+#    #+#             */
/*   Updated: 2024/09/29 00:11:20 by tbabou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_full_cmd(char *bin)
{
    char *path;
    char **paths;
    int i;
    
    i = 0;
    path = getenv("PATH");
    paths = ft_split(path, ':');
    while (paths[i++])
    {
        path = ft_strjoin(paths[i], "/");
        paths[i] = ft_strjoin(path, bin);
    }
    free(path);
    i = 0;
    while (paths[i++])
        if (access(paths[i], F_OK) == 0)
            return (paths[i]);
    return (NULL);
}

void free_minishell(void)
{
    printf("free minishell\n");
}

void exit_error(char *msg)
{
    perror(msg);
    free_minishell();
    exit(EXIT_FAILURE);
}